//
//  PlayerColumn.cpp
//  fftRacers
//
//  Created by Matthew Herrero on 11/25/16.
//
//

#include "PlayerColumn.h"
#include "Racer.h"
#include "ofMain.h"
#include <iostream>
#include <string>
#include <algorithm>

PlayerColumn::~PlayerColumn(){
    delete[] racer;
}

void PlayerColumn::calc_column_dimensions() {
    column_width = ofGetWidth()/4.0;
    column_height = 2*ofGetHeight()/3.0;
    //get power-up lane positions
}

void PlayerColumn::assign_player_specific_fields(int playerNumber) {
    switch(playerNumber) {
        case 1: {
            racer->set_move_keys('a','d');
            gainKey = 'f';
            racer->set_racer_color(0, 0, 255);
            break;
        }
        case 2: {
            racer->set_move_keys(OF_KEY_LEFT, OF_KEY_RIGHT);
            gainKey = 'g';
            racer->set_racer_color(255, 0, 0);
            break;
        }
    }
}

PlayerColumn::PlayerColumn(float* smoothedVolApp, vector<int>* keyStateApp, int playerNumber) :
    smoothedVolPtr(smoothedVolApp),
    keyStatePtr(keyStateApp)
{
    calc_column_dimensions();
    // init power-up values (gain_multiplier, overheat, [sweet spot values])
    gain_multiplier = 1;
    volHistory.assign(VOL_BUFFER_SIZE, make_tuple(0.0, 0.0));
    scaledVol = 0.0;
    // init racer position point
    racer = new Racer(keyStatePtr, column_width, column_height);
    racer->set_racer_position(column_width/2.0, (4*column_height)/5.0);
    assign_player_specific_fields(playerNumber);
}

void PlayerColumn::add_current_volume_val() {
    //lets scale the vol up to a 0-1 range
    float curr_smoothed = *smoothedVolPtr;
    scaledVol = ofMap(curr_smoothed, 0.0, 0.17, 0.0, ((column_width/2.0)-(racer->get_radius()+5))/2.0, true);
    
    //lets record the volume into an array
    volHistory.push_back(make_tuple(scaledVol, gain_multiplier));
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if(volHistory.size() >= VOL_BUFFER_SIZE){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
}

bool PlayerColumn::in_key_state(int key) {
    vector <int> keyState = *keyStatePtr;
    return (find(keyState.begin(), keyState.end(), key) != keyState.end());
}

bool PlayerColumn::update(){
    add_current_volume_val();
    if(check_for_collision()) return true;
    racer->update();
    if(in_key_state(gainKey) && gain_multiplier < 2){
        gain_multiplier += 0.1;
    } else if (gain_multiplier > 1) {
        gain_multiplier -= 0.1;
    }
    return false;
}

void PlayerColumn::resize() {
    float racer_perc_from_left = racer->get_center_x()/column_width;
    calc_column_dimensions();
    racer->resize(racer_perc_from_left, column_width, column_height);
}

void PlayerColumn::clear_danger_buffers(){
    if(danger_zone_left.size() > 0 && danger_zone_right.size() > 0){
        danger_zone_left.erase(danger_zone_left.begin(), danger_zone_left.end());
        danger_zone_right.erase(danger_zone_right.begin(), danger_zone_right.end());
    }
}

void PlayerColumn::draw_volume_walls() {
    clear_danger_buffers();
    ofSetColor(245, 58, 135);
    ofFill();
    float step_size = column_height/volHistory.size();
    //draw volume history on the left
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        float y_position = column_height - (i*step_size);
        float x_position;
        if(i == 0 || i == volHistory.size() -1){
            x_position = 0;
        } else {
            x_position = get<0>(volHistory[i]) * get<1>(volHistory[i]);
        }
        ofVertex(x_position, y_position);
        if (y_position <= racer->get_top_y() + step_size && y_position >= racer->get_bottom_y() - step_size){
            danger_zone_left.push_back(ofPoint(x_position, y_position));
        }
    }
    ofEndShape(false);
    
    //draw volume history on the floor
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        float y_position = column_height - (i*step_size);
        float x_position;
        if(i == 0 || i == volHistory.size() -1){
            x_position = column_width;
        } else {
            x_position = column_width - (get<0>(volHistory[i]) * get<1>(volHistory[i]));
        }
        ofVertex(x_position, y_position);
        if (y_position <= racer->get_top_y() + step_size && y_position >= racer->get_bottom_y() - step_size){
            danger_zone_right.push_back(ofPoint(x_position, y_position));
        }
    }
    ofEndShape(false);
}

void PlayerColumn::draw_border() {
    ofSetColor(245, 58, 135);
    ofNoFill();
    ofDrawRectangle(0, 0, 0, column_width, column_height);
}

void PlayerColumn::draw() {
    draw_border();
    draw_volume_walls();
    racer->draw();
}

vector<ofPoint> PlayerColumn::get_danger_points(vector<ofPoint> danger_zone, float y_coord){
    //for each pair of points in the danger zone
    for(int i = 0; i < danger_zone.size(); i++){
        //if the y_coordinate is between the y values of the danger points
        float upper_y = danger_zone[i].y;
        float lower_y = danger_zone[i+1].y;
        if(y_coord <= upper_y && y_coord >= lower_y){
            vector<ofPoint> danger_points(danger_zone.begin()+i, danger_zone.begin()+i+2);
            return danger_points;
        }
    }
    return vector<ofPoint>(0);
}

bool PlayerColumn::check_for_collision(){
    //for each y in the racer_range
    if(!(danger_zone_left.empty() || danger_zone_right.empty())){
        vector<float> y_range = racer->get_y_range();
        for(int i = 0; i < y_range.size(); i++){
            vector<ofPoint> racer_edges = racer->get_x_edges(y_range[i]);
            vector<ofPoint> left_dangers = get_danger_points(danger_zone_left, y_range[i]);
            vector<ofPoint> right_dangers = get_danger_points(danger_zone_right, y_range[i]);
            for(int i = 0; i < racer_edges.size(); i++) {
                if(racer_edges[i].x >= racer->get_center_x()){
                    if(racer_edges[i].x >= right_dangers[0].x && racer_edges[i].x >= right_dangers[1].x){
                        racer->set_racer_color(255, 255, 255);
                        return true;
                    }
                } else if(racer_edges[i].x <= racer->get_center_x()){
                    if(racer_edges[i].x <= left_dangers[0].x && racer_edges[i].x <= left_dangers[1].x){
                        racer->set_racer_color(255, 255, 255);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

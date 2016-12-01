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
#include <algorithm>

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
    gain_multiplier = 100;
    volHistory.assign(VOL_BUFFER_SIZE, make_tuple(0.0, 0.0));
    scaledVol = 0.0;
    // init racer position point
    racer = new Racer(keyStatePtr, column_width, column_height);
    racer->set_racer_position(column_width/2.0, (4*column_height)/5.0);
    assign_player_specific_fields(playerNumber);
}

void PlayerColumn::add_current_volume_val() {
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(*smoothedVolPtr, 0.0, 0.17, 0.0, 1.0, true);
    
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

void PlayerColumn::update(){
    add_current_volume_val();
    racer->update();
    if(in_key_state(gainKey) && gain_multiplier < 1000){
        gain_multiplier += 10;
    } else if (gain_multiplier > 100) {
        gain_multiplier -= 10;
    }
}

void PlayerColumn::resize() {
    float racer_perc_from_left = racer->get_center_x()/column_width;
    calc_column_dimensions();
    racer->resize(racer_perc_from_left, column_width, column_height);
}

void PlayerColumn::draw_volume_walls() {
    ofSetColor(245, 58, 135);
    ofFill();
    
    float step_size = column_height/volHistory.size();
    
    //draw volume history on the roof
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        float y_position = column_height - (i*step_size);
        if(i == 0 || i == volHistory.size() -1){
            ofVertex(0, y_position);
        } else {
            ofVertex(get<0>(volHistory[i]) * get<1>(volHistory[i]), y_position);
        }
    }
    ofEndShape(false);
    
    //draw volume history on the floor
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        float y_position = column_height - (i*step_size);
        if(i == 0 || i == volHistory.size() -1){
            ofVertex(column_width, y_position);
        } else {
            ofVertex(column_width - (get<0>(volHistory[i]) * get<1>(volHistory[i])), y_position);
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

//
//  PlayerColumn.cpp
//  fftRacers
//
//  Created by Matthew Herrero on 11/25/16.
//
//

#include "PlayerColumn.h"
#include "ofMain.h"
#include <iostream>
#include <algorithm>

void PlayerColumn::calc_column_dimensions() {
    column_width = ofGetWidth()/4.0;
    column_height = 2*ofGetHeight()/3.0;
    //get power-up lane positions
}

void PlayerColumn::assign_movement_keys(int playerNumber) {
    switch(playerNumber) {
        case 1: {
            moveLeftKey = OF_KEY_LEFT;
            moveRightKey = OF_KEY_RIGHT;
            break;
        }
        case 2: {
            moveLeftKey = 'a';
            moveRightKey = 'd';
            break;
        }
    }
}

PlayerColumn::PlayerColumn(float* smoothedVolApp, vector<int>* keyStateApp, int playerNumber) :
    smoothedVolPtr(smoothedVolApp),
    keyStatePtr(keyStateApp)
{
    // calculate column dimensions - width, height, power-up lane
    calc_column_dimensions();
    // init power-up values (gain_multiplier, overheat, [sweet spot values])
    gain_multiplier = 100;
    volHistory.assign(VOL_BUFFER_SIZE, make_tuple(0.0, 0.0));
    scaledVol = 0.0;
    // init racer position point
    racer_posit.set(column_width/2.0, (4*column_height)/5.0);
    assign_movement_keys(playerNumber);
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

void PlayerColumn::update_racer_position() {
    vector <int> keyState = *keyStatePtr;
    if (!(keyState.empty())){
        if (in_key_state(moveRightKey) && racer_posit.x < column_width) racer_posit.x += 4;
        if (in_key_state(moveLeftKey) && racer_posit.x > 0) racer_posit.x -= 4;
    }
}

void PlayerColumn::update(){
    add_current_volume_val();
    update_racer_position();
}

void PlayerColumn::resize_racer_position(float racer_perc_from_left) {
    racer_posit.set(column_width * racer_perc_from_left, (column_height*4)/5.0);
}

void PlayerColumn::resize() {
    float racer_perc_from_left = racer_posit.x/column_width;
    calc_column_dimensions();
    resize_racer_position(racer_perc_from_left);
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

void PlayerColumn::draw_racer() {
    ofDrawCircle(racer_posit, 5);
}

void PlayerColumn::draw_border() {
    ofSetColor(225);
    ofNoFill();
    ofDrawRectangle(0, 0, 0, column_width, column_height);
}

void PlayerColumn::draw() {
    draw_border();
    draw_volume_walls();
    draw_racer();
}

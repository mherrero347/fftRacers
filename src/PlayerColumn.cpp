//
//  PlayerColumn.cpp
//  fftRacers
//
//  Created by Matthew Herrero on 11/25/16.
//
//

#include "PlayerColumn.h"
#include "ofMain.h"

void PlayerColumn::calc_column_dimensions() {
    column_width = ofGetWidth()/4.0;
    column_height = 2*ofGetHeight()/3.0;
    //get power-up lane positions
}

PlayerColumn::PlayerColumn(float* smoothedVolApp) : smoothedVolPtr(smoothedVolApp) {
    // calculate column dimensions - width, height, power-up lane
    calc_column_dimensions();
    // init power-up values (gain_multiplier, overheat, [sweet spot values])
    gain_multiplier = 100;
    // init volumeHistory vector
    
    volHistory.assign(VOL_BUFFER_SIZE, make_tuple(0.0, 0.0));
    scaledVol = 0.0;
    // init racer position point
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

void PlayerColumn::update(){
    add_current_volume_val();
}

void PlayerColumn::resize() {
    calc_column_dimensions();
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

void PlayerColumn::draw() {
    ofDrawRectangle(0, 0, column_width, column_height);
    draw_volume_walls();
}

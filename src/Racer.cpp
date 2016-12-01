//
//  Racer.cpp
//  fftRacers
//
//  Created by Matthew Herrero on 11/30/16.
//
//

//#include <iostream>
//#include <string>
#include <cmath>
#include "Racer.h"
#include "ofMain.h"

Racer::Racer(vector<int>* keyStatePtrColumn, int width, int height) :
    keyStatePtr(keyStatePtrColumn),
    column_width(width),
    column_height(height),
    move_left_key(0),
    move_right_key(0),
    current_accel(0),
    current_vel(0)
{
    racer_posit.x = 0;
    racer_posit.y = 0;
}

void Racer::set_racer_color(int r, int g, int b){
    racer_color.r = r;
    racer_color.g = g;
    racer_color.b = b;
}

void Racer::set_racer_position(float x, float y){
    racer_posit.x = x;
    racer_posit.y = y;
}

void Racer::update(){
    update_racer_position();
    cout << "accel = " + to_string(current_accel) << endl;
    cout << "vel = " + to_string(current_vel) << endl;
}

void Racer::draw(){
    ofSetColor(racer_color);
    //Change to racer_radius
    ofDrawCircle(racer_posit, 10);
}

void Racer::resize(float racer_perc_from_left, int new_width, int new_height){
    column_width = new_width;
    column_height = new_height;
    racer_posit.set(column_width * racer_perc_from_left, (column_height*4)/5.0);
}

void Racer::get_racer_edge(float y_level){
    
}

void Racer::update_racer_position(){
    update_accel_and_vel();
    // FIX CATCHING ON EDGES BUG
    if (racer_posit.x < column_width && racer_posit.x > 0) {
        racer_posit.x += current_vel;
    }
}

void Racer::update_accel_and_vel(){
    vector <int> keyState = *keyStatePtr;
    if (in_key_state(move_right_key) || in_key_state(move_left_key)){
        current_accel = move_accel;
        if (in_key_state(move_left_key)) current_accel *= -1;
    } else if (current_vel != 0){
        current_accel = damping_accel;
        if (current_vel > 0) current_accel *= -1;
        if (abs(current_vel) < abs(current_accel)){
            current_accel = 0;
            current_vel = 0;
        }
    } else {
        current_accel = 0;
    }
    current_vel += current_accel;
}

bool Racer::in_key_state(int key) {
    vector <int> keyState = *keyStatePtr;
    return (find(keyState.begin(), keyState.end(), key) != keyState.end());
}

float Racer::get_center_x() {
    return racer_posit.x;
}

void Racer::set_move_keys(int left, int right) {
    move_left_key = left;
    move_right_key = right;
}

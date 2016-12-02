//
//  Racer.hpp
//  fftRacers
//
//  Created by Matthew Herrero on 11/30/16.
//
//

#ifndef Racer_h
#define Racer_h

#include <stdio.h>
#include "ofMain.h"

class Racer {
    
public:
    Racer(vector<int>* keyStatePtrColumn, int width, int height);
    void set_move_keys(int left, int right);
    void set_racer_color(int r, int g, int b);
    void set_racer_position(float x, float y);
    void update();
    void draw();
    void resize(float racer_perc_from_left, int new_width, int new_height);
    vector <ofPoint> get_x_edges(float y_level);
    float get_center_x();
    float get_center_y();
    vector<float> get_y_range();
    float get_top_y();
    float get_bottom_y();
    
private:
    void update_racer_position();
    void update_accel_and_vel();
    bool in_key_state(int key);
    
    ofPoint racer_posit;
    ofColor racer_color;
    int move_left_key, move_right_key;
    float column_width, column_height, current_accel, current_vel;
    vector<int>* keyStatePtr;
    const int racer_radius = 10;
    const float move_accel = .8;
    const float damping_accel = .3;
    const float velocity_epsilon = 0.1;
    
};

#endif /* Racer_h */

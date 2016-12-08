/* File: Racer.h - fftRacers - Created by Matthew Herrero on 11/30/16.
 ------------------------------------------------------------------------------
 * This file implements a racer object for use in PlayerColumn object. This
 * Racer class tracks its own position, radius, and color. It implements an
 * acceleration-velocity based movement scheme. It contains some helper function
 * for the check_collisions function in PlayerColumn, such as getting all y
 * levels the circle is contained on, the circle's edge points at a given y value,
 * and the center points for the circle.
 *
 * Detailed function descriptions can be found in the Racer.cpp file
 */

#ifndef Racer_h
#define Racer_h

#include <stdio.h>
#include "ofMain.h"

class Racer {
    
public:
    Racer(vector<int>* _keyStatePtr, int _column_width, int _column_height);
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
    int get_radius();
    
private:
    void update_racer_position();
    void update_accel_and_vel();
    bool in_key_state(int key);
    
    ofPoint racer_posit;
    ofColor racer_color;
    int move_left_key, move_right_key;
    float column_width, column_height, current_accel, current_vel;
    //contains a pointer to the vector containing which keys are held down, owned and updated by ofApp
    vector<int>* keyStatePtr;
    
    //CONSTANTS
    const int racer_radius = 10;
    const float move_accel = .8;
    const float max_velocity = 3;
    const float damping_accel = .5;
    const float velocity_epsilon = 0.1;
    const float vel_jitter_const = 1.2;
    
};

#endif /* Racer_h */

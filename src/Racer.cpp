/* File: Racer.cpp - fftRacers - Created by Matthew Herrero on 11/30/16.
 ------------------------------------------------------------------------------
 * This file implements a racer object for use in PlayerColumn object. This
 * Racer class tracks its own position, radius, and color. It implements an
 * acceleration-velocity based movement scheme. It contains some helper function
 * for the check_collisions function in PlayerColumn, such as getting all y
 * levels the circle is contained on, the circle's edge points at a given y value,
 * and the center points for the circle.
 */

#include <cmath>
#include "Racer.h"
#include "ofMain.h"

/* --------------------------------------------------------------
                        PUBLIC METHODS
 -------------------------------------------------------------- */

/* Constructor: Racer
 -------------------------------
 * This is the constructor for the Racer class. It takes in a pointer to the 
 * keyState vector (owned by ofApp), the width of the column, and the height
 * of the column. It contains stub intialization for move_left_key, move_right_key
 * current_accel, current_vel, and the racer position object
 */
Racer::Racer(vector<int>* _keyStatePtr, int _column_width, int _column_height) :
keyStatePtr(_keyStatePtr),
column_width(_column_width),
column_height(_column_height),
move_left_key(0),
move_right_key(0),
current_accel(0),
current_vel(0)
{
    racer_posit.x = 0;
    racer_posit.y = 0;
}

/* Function: set_move_keys
 -------------------------------
 * This function stores the keys that the racer will move in response to. Key
 * values are respresented by int values.
 */
void Racer::set_move_keys(int left, int right) {
    move_left_key = left;
    move_right_key = right;
}

/* Function: set_racer_color
 -------------------------------
 * This function stores the color of the racer
 */
void Racer::set_racer_color(int r, int g, int b){
    racer_color.r = r;
    racer_color.g = g;
    racer_color.b = b;
}

/* Function: set_racer_position
 -------------------------------
 * This function sets the x and y position of the racer
 */
void Racer::set_racer_position(float x, float y){
    racer_posit.x = x;
    racer_posit.y = y;
}

/* Function: update
 -------------------------------
 * This is the update function of the racer. This should be called once every
 * graphic-rate cycle (aka, once every time PlayerColumn is updated). This function
 * ensures the racer is in the correct postion on the screen, given the current
 * keyState, by calling the update_racer_position function.
 */
void Racer::update(){
    update_racer_position();
}

/* Function: draw
 -------------------------------
 * This is the draw function for the racer. This should be called once every
 * graphic-rate cycle (aka, once every time PlayerColumn is re-drawn). It draws
 * a circle with radius racer_radius at the current racer position.
 */
void Racer::draw(){
    ofSetColor(racer_color);
    ofDrawCircle(racer_posit, racer_radius);
}

/* Function: resize
 -------------------------------
 * This is the resize function for the racer. It takes care of moving the racer to
 * the correct relative place in the resized PlayerColumn so that racer stays approximately
 * the same relative distance from the left edge of the column (ignoring volume walls)
 */
void Racer::resize(float racer_perc_from_left, int new_width, int new_height){
    column_width = new_width;
    column_height = new_height;
    racer_posit.set(column_width * racer_perc_from_left, (column_height*4)/5.0);
}

/* Function: get_x_edges
 -------------------------------
 * This function returns a vector of ofPoints which contain the points on the circumfrence of
 * the racer circle that lie on a given y level (passed in as a parameter). For example, if
 * y_level = 10 is passed into the function, and there are two points on the edge of the circle
 * whose y values are 10 (ie - if the center of the circle on the y=10 line), then those two points
 * will put into the points vector and returned. Only one point is returned if the y_level is at
 * the very top or bottom of the racer circle.
 */
vector <ofPoint> Racer::get_x_edges(float y_level){
    vector <ofPoint> points;
    float dist_from_center = sqrt(pow(racer_radius, 2) - pow(abs(racer_posit.y - y_level), 2));
    points.push_back(ofPoint(racer_posit.x-dist_from_center, y_level));
    if(dist_from_center > 0) points.push_back(ofPoint(racer_posit.x+dist_from_center, y_level));
    return points;
}

/* Function: get_center_x
 -------------------------------
 * Returns the x value at the center of the racer
 */
float Racer::get_center_x() {
    return racer_posit.x;
}

/* Function: get_center_y
 -------------------------------
 * Returns the y value at the center of the racer
 */
float Racer::get_center_y() {
    return racer_posit.y;
}

/* Function: get_y_range
 -------------------------------
 * Returns the range of y values (relative to the PlayerColumn owner) that span
 * from the top of the racer circle to the bottom
 */
vector<float> Racer::get_y_range(){
    vector<float> y_range;
    for(float i = racer_posit.y - racer_radius; i <= racer_posit.y + racer_radius; i++){
        y_range.push_back(i);
    }
    return y_range;
}

/* Function: get_top_y
 -------------------------------
 * Returns the y value (relative to the PlayerColumn owner) that is at the top
 * of the racer circle
 */
float Racer::get_top_y(){
    return racer_posit.y + racer_radius;
}

/* Function: get_bottom_y
 -------------------------------
 * Returns the y value at the bottom of the racer circle
 */
float Racer::get_bottom_y(){
    return racer_posit.y - racer_radius;
}

/* Function: get_radius
 -------------------------------
 * Returns the radius of the racer circle
 */
int Racer::get_radius(){
    return racer_radius;
}

/* --------------------------------------------------------------
                        PRIVATE METHODS
 -------------------------------------------------------------- */

/* Function: update_racer_position
 -------------------------------
 * This function updates the racer position. This should be called between
 * draw calls. First, it updates the acceleration and velocity values of the
 * racer using the update_accel_and_vel method, and then updates the racer's x 
 * value based on the current velocity
 */
void Racer::update_racer_position(){
    update_accel_and_vel();
    racer_posit.x += current_vel;
}

/* Function: update_accel_and_vel
 -------------------------------
 * This function updates the acceleration and velocity of the racer. This should
 * be called between draw calls. It first updates the acceleration, setting it to
 * positive move_accel if the right move key is currently down, negative move_accel
 * if the left move key is currently down, postive damping_accel if neither is down
 * and the racer has leftward velocity, and negative damping_accel if neither is down
 * and the racer has rightward velocity. If the current velocity is less than the
 * acceleration after this update, then the racer should stop, and the acceleration is
 * reset to 0, along with the velocity. Then, the overall velocity is updated, and a 
 * random jitter is applied to give the racer a little random movement
 */
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
    current_vel += current_accel  + ofRandom(-vel_jitter_const, vel_jitter_const);
}

/* Function: in_key_state
 -------------------------------
 * This function returns true if the given key (key parameter) is inside the 
 * current key state, and false if not.
 */
bool Racer::in_key_state(int key) {
    vector <int> keyState = *keyStatePtr;
    return (find(keyState.begin(), keyState.end(), key) != keyState.end());
}

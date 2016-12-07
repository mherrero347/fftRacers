/* File: PlayerColumn.cpp - fftRacers - Created by Matthew Herrero on 11/25/16.
 ------------------------------------------------------------------------------
 *
 */

#include "PlayerColumn.h"
#include "Racer.h"
#include "ofMain.h"
#include <algorithm>

/* --------------------------------------------------------------
                        PUBLIC METHODS
 -------------------------------------------------------------- */

/* Destructor: ~PlayerColumn
 -------------------------------
 * When the PlayerColumn object is destroyed, the malloced poitner to the racer
 * object is cleaned and removed
 */
PlayerColumn::~PlayerColumn(){
    delete[] racer;
}

/* Constructor: PlayerColumn
 -------------------------------
 * This is the constructor of the PlayerColumn object. It takes in 4 parameters, each of which
 * are used to initialize a different member variables in the class:
 *      - smoothedVolApp points to a float value in ofApp that is updated with smoothed
 *          rms volume of the signal at each moment (audio-thread)
 *      - keyStateApp points to the vector containing keys currently pressed down
 *      - isClippingApp points to a vector of booleans. The first element tells if player
 *          one has pegged their current FFT bin, and the second tells if player two
 *          has done that
 *      - _playerNumber contains the current player number
 * 
 * This constructor initializes gain_multiplier, scaledVol, and the volHistory vector. It
 * also intializes the racer object, sets it to it's initial position, and assigns the player
 * specific values (move keys and color)
 */
PlayerColumn::PlayerColumn(float* smoothedVolApp, vector<int>* keyStateApp, vector<bool>* isClippingArrApp, int _playerNumber) :
smoothedVolPtr(smoothedVolApp),
keyStatePtr(keyStateApp),
isClippingPtr(isClippingArrApp),
playerNumber(_playerNumber)
{
    calc_column_dimensions();
    gain_multiplier = 1;
    volHistory.assign(VOL_BUFFER_SIZE, make_tuple(0.0, 0.0));
    scaledVol = 0.0;
    racer = new Racer(keyStatePtr, column_width, column_height);
    racer->set_racer_position(column_width/2.0, (4*column_height)/5.0);
    assign_player_specific_fields(playerNumber);
}

/* Function: update
 -------------------------------
 * This is the update function for the PlayerColumn. It returns a bool that reports if a collision
 * has occured as a result of the most recent draw execution. First, it updates the volHistory
 * vector and checks for a collision. If no collision has occured, the function updates the
 * racer object. Finally, it checks the opponent player is pegging out their current fft bin,
 * and increments/decrements the gain_multiplier value accordingly.
 */
bool PlayerColumn::update(){
    add_current_volume_val();
    if(check_for_collision()) return true;
    racer->update();
    vector<bool> isClipping = *isClippingPtr;
    if(isClipping[playerNumber%2] && gain_multiplier < 2){
        gain_multiplier += 0.02;
    } else if (gain_multiplier > 1) {
        gain_multiplier -= 0.02;
    }
    return false;
}

/* Function: resize
 -------------------------------
 * This is the resize function for the PlayerColumn. It calculates what relative distance the
 * racer is from the left wall, recalculates the column height and width, and then uses the
 * racer's resize function to place the racer in the correct position in the new column
 */
void PlayerColumn::resize() {
    float racer_perc_from_left = racer->get_center_x()/column_width;
    calc_column_dimensions();
    racer->resize(racer_perc_from_left, column_width, column_height);
}

/* Function: draw
 -------------------------------
 * This is the draw function for the PlayerColumn class. It first draws the borders of the 
 * column, then the volume-based column walls, then the racer object.
 */
void PlayerColumn::draw() {
    draw_border();
    draw_volume_walls();
    racer->draw();
}

/* Function: check_for_collision
 -------------------------------
 * This function checks if the racer is currently colliding with the volume walls. It returns
 * true if there is a collision occuring, false if not. The vectors danger_zone_left and 
 * danger_zone_right are updated and maintained in the draw_volume_walls function
 */
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

/* --------------------------------------------------------------
                        PRIVATE METHODS
 -------------------------------------------------------------- */

/* Function: calc_column_dimensions
 -------------------------------
 * This function calculates the correct width and height of the column, relative
 * to window dimensions
 */
void PlayerColumn::calc_column_dimensions() {
    column_width = ofGetWidth()/3.0;
    column_height = 2*ofGetHeight()/3.0;
}

/* Function: assign_player_specific_fields
 -------------------------------
 * This function takes in an int relating to the number of the column's player
 * and assigns the correct player-specific racer color and move keys
 */
void PlayerColumn::assign_player_specific_fields(int playerNumber) {
    switch(playerNumber) {
        case 1: {
            racer->set_move_keys('a','d');
            racer->set_racer_color(0, 0, 255);
            break;
        }
        case 2: {
            racer->set_move_keys(OF_KEY_LEFT, OF_KEY_RIGHT);
            racer->set_racer_color(255, 0, 0);
            break;
        }
    }
}

/* Function: add_current_volume_val
 -------------------------------
 * This updates the volHistory vector with the current smoothed rms volume of the 
 * incoming signal, and shifts the vector values, deleting the least recent if needed.
 */
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

/* Function: in_key_state
 -------------------------------
 * This function checks if a key is in the current key state (keys pressed down)
 */
bool PlayerColumn::in_key_state(int key) {
    vector <int> keyState = *keyStatePtr;
    return (find(keyState.begin(), keyState.end(), key) != keyState.end());
}

/* Function: clear_danger_buffers
 -------------------------------
 * This function clears the danger_zone_left and danger_zone_right vectors. If these
 * vectors are not yet initialized, this method do
 */
void PlayerColumn::clear_danger_buffers(){
    if(danger_zone_left.size() > 0 && danger_zone_right.size() > 0){
        danger_zone_left.erase(danger_zone_left.begin(), danger_zone_left.end());
        danger_zone_right.erase(danger_zone_right.begin(), danger_zone_right.end());
    }
}

/* Function: draw_volume_walls
 -------------------------------
 * This function draws the volume walls on the sides of the column, as well as keeping
 * the danger_zone vectors updated with the ofPoint's representing the points where it's
 * possible that the racer might run into a wall.
 */
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

/* Function: draw_border
 -------------------------------
 * This function draws the border around the edge of the column
 */
void PlayerColumn::draw_border() {
    ofSetColor(245, 58, 135);
    ofNoFill();
    ofDrawRectangle(0, 0, 0, column_width, column_height);
}

/* Function: get_danger_points
 -------------------------------
 * This function takes in a vector of ofPoints, which is actual one of the danger_zone
 * member vectors, as well as a y coordinate, and returns a vector containg the two 
 * ofPoints in the danger_zone that are on either side of the passed y_coord (the y_coord is
 * minimally less than the y coordinate of one of the ofPoints danger points, and minimally 
 * greater than the other).
 */
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

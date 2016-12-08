/* File: PlayerColumn.h - fftRacers - Created by Matthew Herrero on 11/25/16.
 ------------------------------------------------------------------------------
 * This is the PlayerColumn class that creates and handles a racer column for
 * a player. It draws the walls of the column to reflect the recent rms volume
 * from the microphone input. It checks for whether the racer object has collided
 * with the wall at each moment, and signals the ofApp object if that is the
 * case. It also handles assigning racer-specific properties, like racer
 * color and move keys.
 * 
 * Detailed function documentation can be found in PlayerColumn.cpp
 */

#ifndef PlayerColumn_h
#define PlayerColumn_h

#include <stdio.h>
#include "ofMain.h"
#include "Racer.h"

class PlayerColumn {
    PlayerColumn();
    ~PlayerColumn();
    
    public:
        PlayerColumn(float* _smoothedVolPtr, vector<int>* _keyStatePtr, vector<bool>* _isClippingPtr, int _playerNumber);
        bool update();
        void draw();
        void resize();
        bool check_for_collision();

    private:
        void calc_column_dimensions();
        void draw_border();
        void add_current_volume_val();
        void draw_volume_walls();
        bool in_key_state(int key);
        void assign_player_specific_fields(int playerNumber);
        vector<ofPoint> get_danger_points(vector<ofPoint> danger_zone, float y_coord);
        void clear_danger_buffers();
    
        //pointer to the racer object in the column, the circle controlled by the player
        Racer *racer;
        int playerNumber;
        /* this vector contains the rms volume history of the microphone input, for use in
         * drawing the column walls. Each element is a tuple, with the first tuple member being
         * the rms volume at that moment, and the second being the gain_multiplier at that
         * moment, which increments and decriments with whether the opponent has been
         * clipping out their assigned fft bin */
        vector <tuple <float,float>> volHistory;
        //these vector are points based on subsets of the volHistory vector, and they are points
        //on the volume history walls where the racer object could possibly collide with, one for
        //left wall, the other for the right.
        vector <ofPoint> danger_zone_left;
        vector <ofPoint> danger_zone_right;
        float scaledVol, gain_multiplier, column_width, column_height;
        //this points to the smoothedVol float value updated in ofApp's audioIn function
        float* smoothedVolPtr;
        //this points to ofApp's key_state_arr vector
        vector<int>* keyStatePtr;
        //this points to ofApp's isClipping vector.
        vector<bool>* isClippingPtr;
    
        const int VOL_BUFFER_SIZE = 50;
    
};

#endif /* PlayerColumn_h */

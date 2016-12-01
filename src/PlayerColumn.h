//
//  PlayerColumn.h
//  fftRacers
//
//  Created by Matthew Herrero on 11/25/16.
//
//

#ifndef PlayerColumn_h
#define PlayerColumn_h

#include <stdio.h>
#include "ofMain.h"

class PlayerColumn {
    
    public:
        PlayerColumn(float* smoothedVolApp, vector<int>* keyStateApp, int playerNumber);
        void update();
        void draw();
        void resize();

    private:
        void calc_column_dimensions();
        void draw_border();
        void add_current_volume_val();
        void update_racer_position();
        void resize_racer_position(float racer_perc_from_left);
        void draw_racer();
        void draw_volume_walls();
        bool in_key_state(int key);
        void update_racer_accel();
        void assign_player_specific_fields(int playerNumber);
    
        ofPoint racer_posit;
        ofColor racer_color;
        int moveLeftKey, moveRightKey, gainKey;
        vector <tuple <float,float>> volHistory;
        float scaledVol, gain_multiplier, column_width, column_height, current_accel, current_vel;
        float* smoothedVolPtr;
        vector<int>* keyStatePtr;
        const int VOL_BUFFER_SIZE = 150;
        const float move_accel = 1;
        const float damping_accel = .4;
    
};

#endif /* PlayerColumn_h */

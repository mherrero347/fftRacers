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
#include "Racer.h"

class PlayerColumn {
    PlayerColumn();
    ~PlayerColumn();
    
    public:
        PlayerColumn(float* smoothedVolApp, vector<int>* keyStateApp, vector<bool>* isClippingArrApp, int playerNumber);
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
    
        Racer *racer;
        int playerNumber;
        vector <tuple <float,float>> volHistory;
        vector <ofPoint> danger_zone_left;
        vector <ofPoint> danger_zone_right;
        float scaledVol, gain_multiplier, column_width, column_height;
        float* smoothedVolPtr;
        vector<int>* keyStatePtr;
        vector<bool>* isClippingPtr;
        const int VOL_BUFFER_SIZE = 50;
    
};

#endif /* PlayerColumn_h */

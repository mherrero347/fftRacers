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
    
    public:
        PlayerColumn(float* smoothedVolApp, vector<int>* keyStateApp, int playerNumber);
        void update();
        void draw();
        void resize();

    private:
        void calc_column_dimensions();
        void draw_border();
        void add_current_volume_val();
        void draw_volume_walls();
        bool in_key_state(int key);
        void assign_player_specific_fields(int playerNumber);
    
        Racer *racer;
        int gainKey;
        vector <tuple <float,float>> volHistory;
        float scaledVol, gain_multiplier, column_width, column_height;
        float* smoothedVolPtr;
        vector<int>* keyStatePtr;
        const int VOL_BUFFER_SIZE = 150;
    
};

#endif /* PlayerColumn_h */

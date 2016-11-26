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
        PlayerColumn(float* smoothedVolApp);
        void update();
        void draw();
        void resize();

    private:
        void calc_column_dimensions();
        void add_current_volume_val();
        void draw_volume_walls();
    
        vector <tuple <float,float>> volHistory;
        float scaledVol, gain_multiplier;
        float* smoothedVolPtr;
        float column_width, column_height;
        const int VOL_BUFFER_SIZE = 200;
};

#endif /* PlayerColumn_h */

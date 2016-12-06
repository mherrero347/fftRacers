//
//  FftBar.h
//  fftRacers
//
//  Created by Matthew Herrero on 12/4/16.
//
//

#ifndef FftBar_h
#define FftBar_h

#include <stdio.h>
#include <functional>
#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "ofxSimpleTimer.h"

class FftBar {
    
public:
    FftBar();
    bool update();
    void draw();
    void resize();
    void storeAppAudioCallback(std::function<void(vector<float>, int, int)> _audioInApp);
    bool playerClipping(int player);
    
private:
    void calc_dimensions();
    vector<float> get_game_spectrum();
    void draw_bin_bars();
    bool bin_passed_thresh(int ind);
    void changePlayerBars(int &i);
    
    ProcessFFT fft;
    ofxSimpleTimer player_bar_timer;
    float bar_width, bar_height;
    int currBinPlayerOne, currBinPlayerTwo, game_bins;
    
    const int TRASH_PREFIX_BINS = 11;
};


#endif /* FftBar_h */

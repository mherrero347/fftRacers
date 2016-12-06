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

class FftBar {
    
public:
    FftBar();
    bool update();
    void draw();
    void resize();
    void storeAppAudioCallback(std::function<void(float*, int, int)> _audioInApp);
    
private:
    void calc_dimensions();
    void draw_bin_bars();
    
    ProcessFFT fft;
    float bar_width, bar_height;
};


#endif /* FftBar_h */

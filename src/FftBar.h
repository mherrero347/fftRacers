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
#include "ofMain.h"
#include "ofxProcessFFT.h"

template<class T>
class FftBar {
    
public:
    FftBar(T *parent);
    bool update();
    void draw();
    void resize();
    void storeAppPtrForCallback(void (T::*appAudioIn)(float*, int, int));
    
private:
    void calc_dimensions();
    void draw_bin_bars();
    
    ProcessFFT fft;
    float bar_width, bar_height;
    T* appPtr;
};


#endif /* FftBar_h */

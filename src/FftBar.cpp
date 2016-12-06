//
//  FftBar.cpp
//  fftRacers
//
//  Created by Matthew Herrero on 12/4/16.
//
//

#include "FftBar.h"

//##########################
//#### PUBLIC FUNCTIONS ####
//##########################

template <class T>
FftBar<T>::FftBar(T *parent){
    appPtr = parent;
    calc_dimensions();
    fft.setup();
}

template <class T>
bool FftBar<T>::update(){
    fft.update();
}

template <class T>
void FftBar<T>::draw(){
    ofPushStyle();
    ofSetColor(245, 58, 135);
    ofNoFill();
    ofDrawRectangle(0, 0, bar_width, bar_height);
    draw_bin_bars();
    ofPopStyle();
}

template <class T>
void FftBar<T>::resize(){
    calc_dimensions();
}

template <class T>
void FftBar<T>::storeAppPtrForCallback(void (T::*appAudioIn)(float*, int, int)){
    
}

//###########################
//#### PRIVATE FUNCTIONS ####
//###########################

template <class T>
void FftBar<T>::calc_dimensions() {
    bar_width = 5*ofGetWidth()/6.0;
    bar_height = ofGetHeight()/6.0;
}

template <class T>
void FftBar<T>::draw_bin_bars(){
    //not normalized
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetLineWidth(2);
    vector<float> spectrum = fft.getSpectrum();
    for(int i=0; i<spectrum.size(); i++){ //for the number of columns
        if (i==fft.getLoudBand()) {
            ofSetColor(255,0,0);
        }
        else{
            ofSetColor(100,100,200);
        }
        ofNoFill();
        ofDrawRectangle(bar_width*((float)i/fft.getNumFFTbins()), bar_height-20, bar_width/fft.getNumFFTbins(), -ofMap(spectrum[i], 0, fft.getVolumeRange(), 0, bar_height -50));
    }
    ofPopStyle();
}

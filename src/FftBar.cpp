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

FftBar::FftBar(){
    calc_dimensions();
    fft.setup();
    fft.setNumFFTBins(15);
    fft.setFFTpercentage(.04);
    fft.setVolumeRange(bar_height);
    currBinPlayerOne = ofRandom(0, 15)/1;
    currBinPlayerTwo = ofRandom(0, 15)/1;
}

bool FftBar::update(){
    fft.update();
}

void FftBar::draw(){
    ofPushStyle();
    ofSetColor(245, 58, 135);
    ofNoFill();
    ofDrawRectangle(0, 0, bar_width, bar_height);
    draw_bin_bars();
    ofPopStyle();
}

void FftBar::resize(){
    calc_dimensions();
}

void FftBar::storeAppAudioCallback(std::function<void(vector<float>, int, int)> _audioInApp){
    fft.storeAppAudioCallback(_audioInApp);
}

//###########################
//#### PRIVATE FUNCTIONS ####
//###########################

void FftBar::calc_dimensions() {
    bar_width = 5*ofGetWidth()/6.0;
    bar_height = ofGetHeight()/6.0;
}

bool FftBar::bin_passed_thresh(int ind){
    if(fft.getSpectrum().size()<=0) return false;
    return ofMap(fft.getSpectrum()[ind], 0, fft.getVolumeRange(), 0, 1, true) >= 0.96;
}

void FftBar::draw_bin_bars(){
    //not normalized
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetLineWidth(2);
    vector<float> spectrum = fft.getSpectrum();
    for(int i=0; i<spectrum.size(); i++){ //for the number of columns
        if (i==currBinPlayerOne) {
            ofSetColor(0,0,255);
        }
        else if (i==currBinPlayerTwo){
            ofSetColor(255,0,0);
        } else {
            ofSetColor(128);
        }
        if (bin_passed_thresh(i)) {
            ofFill();
        } else {
            ofNoFill();
        }
        ofDrawRectangle(bar_width*((float)i/(fft.getNumFFTbins())), bar_height, bar_width/(fft.getNumFFTbins()), -bar_height*ofMap(spectrum[i], 0, fft.getVolumeRange(), 0, 1, true));
    }
    ofPopStyle();
}

bool FftBar::playerClipping(int player){
    if(player == 1) {
        return bin_passed_thresh(currBinPlayerOne);
    } else {
        return bin_passed_thresh(currBinPlayerTwo);
    }
    return false;
}

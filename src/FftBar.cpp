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
    fft.setNumFFTBins(50);
    fft.setNormalize(false);
    fft.setFFTpercentage(.05);
    fft.setVolumeRange(400);
    player_bar_timer.setup(5000);
    player_bar_timer.start(true);
    currBinPlayerOne = ofRandom(0, 15)/1;
    currBinPlayerTwo = ofRandom(0, 15)/1;
    ofAddListener(player_bar_timer.TIMER_COMPLETE, this, &FftBar::changePlayerBars);
    game_bins = 0;
}

bool FftBar::update(){
    fft.update();
    player_bar_timer.update();
}

void FftBar::changePlayerBars(int &i){
    currBinPlayerOne = ofRandom(0, game_bins)/1;
    currBinPlayerTwo = ofRandom(0, game_bins)/1;
}

void FftBar::draw(){
    ofPushStyle();
    ofSetColor(245, 58, 135);
    ofNoFill();
    ofDrawRectangle(0, 0, bar_width, bar_height);
    draw_bin_bars();
    fft.drawDebug();
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
    vector<float> spectrum = get_game_spectrum();
    if(spectrum.size()<=0) return false;
    if(!fft.getNormalized()){
        return ofMap(spectrum[ind], 0, fft.getVolumeRange(), 0, 1, true) >= 0.96;
    } else {
        return spectrum[ind] >= 0.96;
    }
}


vector<float> FftBar::get_game_spectrum(){
    vector<float> bins = fft.getSpectrum();
    if(bins.size() > 0) {
        bins.erase(bins.begin(), bins.begin() + TRASH_PREFIX_BINS+1);
        game_bins = bins.size();
    }
    return bins;
}

void FftBar::draw_bin_bars(){
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetLineWidth(2);
    vector<float> spectrum = get_game_spectrum();
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
        ofDrawRectangle(bar_width*((float)i/game_bins), bar_height, bar_width/(game_bins), -bar_height*ofMap(spectrum[i], 0, fft.getVolumeRange(), 0, 1, true));
    }
    ofPopStyle();
}

bool FftBar::playerClipping(int player){
    if(player == 1) {
        return bin_passed_thresh(currBinPlayerTwo);
    } else {
        return bin_passed_thresh(currBinPlayerOne);
    }
    return false;
}

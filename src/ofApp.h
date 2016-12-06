#pragma once

#include "ofMain.h"
#include "PlayerColumn.h"
#include "FftBar.h"
#include <functional>

class ofApp : public ofBaseApp{
    
public:
    
    //ofApp();
    //~ofApp();
    
    void setup();
    void update();
    void draw();
    
    void draw_game_over_box();
    void game_over_message();
    
    void keyPressed(int key);
    void removeKeyFromState(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
				
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left, right, volume_array, gain_array;
    vector <int> key_state_arr;
    float smoothedVol;
    bool game_over;
    int loser;
    
    vector <PlayerColumn*> column_array;
    FftBar fft_bar;
    ofTrueTypeFont hanzipen_50pt;
    //ofSoundStream soundStream;
    const int NUM_PLAYERS = 2;
};

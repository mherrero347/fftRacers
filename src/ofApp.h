#pragma once

#include "ofMain.h"
#include "PlayerColumn.h"
#include "FftBar.h"
#include <functional>

class ofApp : public ofBaseApp{
    
public:
    
    ~ofApp();
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void removeKeyFromState(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    
    void draw_game_over_box();
    void game_over_message();
    void audioIn(vector<float> input, int bufferSize, int nChannels);
    
    vector <PlayerColumn*> column_array;
    FftBar fft_bar;
    ofTrueTypeFont hanzipen_50pt;
    
    vector <float> left, right;
    vector <int> key_state_arr;
    vector <bool> isClippingArr;
    
    float smoothedVol;
    bool game_over;
    int loser;
    
    const int NUM_PLAYERS = 2;
};

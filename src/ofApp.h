/* File: ofApp.h - created by Matt Herrero
 --------------------------------------------
 * This is the ofApp file, which is the overall controller of the whole game.
 * it creates the player column and fft bar objects, and handles calling their
 * draw and update functions. It also recieves keyboard events and tracks which
 * keys are being held down at each given moment, for use in the PlayerColumn and
 * Racer objects. It also has an audioIn function, which is a sort of psuedo-
 * audio callback (called by the actual audio callback in ofxEasyFFT) and handles
 * calculating the rms volume of each input buffer. It also recognizes and handles
 * game over events.
 * 
 * Detailed function descriptions can be found in ofApp.cpp
 */

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
    
    //contains all pointers to all player column objects
    vector <PlayerColumn*> column_array;
    FftBar fft_bar;
    //this is used to write the final "Red Wins" or "Blue Wins" message
    //in HanziPen, 50pt font at the end of the game
    ofTrueTypeFont hanzipen_50pt;
    
    vector <float> left, right;
    //contains array of all keys currently being pressed
    vector <int> key_state_arr;
    // Element at index 0 reflex if player 1 is currently clipping their
    // fft bin, element at index 1 tells the same for player 2
    vector <bool> isClippingArr;
    
    float smoothedVol;
    bool game_over;
    int loser;
    
    const int NUM_PLAYERS = 2;
};

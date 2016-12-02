#pragma once

#include "ofMain.h"
#include "PlayerColumn.h"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void draw_game_over_box();
    void game_over_message();
    
    void keyPressed(int key);
    void removeKeyFromState(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
				
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left, right, volume_array, gain_array;
    vector <int> key_state_arr;
    float smoothedVol;
    bool game_over;
    int loser;
    
    vector <PlayerColumn*> column_array;
    ofTrueTypeFont hanzipen_50pt;
    ofSoundStream soundStream;
    const int NUM_PLAYERS = 2;
};

#include "ofApp.h"
using namespace std::placeholders;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofBackground(12, 12, 12);
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    int bufferSize = 256;
    smoothedVol = 0.0;
    game_over = false;
    loser = -1;
    hanzipen_50pt.load("HanziPen TC", 50);
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volume_array.assign(NUM_PLAYERS, 0.0);
    
    for(int i = 0; i < NUM_PLAYERS; i++) {
        column_array.push_back(new PlayerColumn(&volume_array[i], &key_state_arr, i+1));
    }
    
    auto audioInAuto = std::bind(&ofApp::audioIn, this, _1, _2, _3);
    fft_bar.storeAppAudioCallback(audioInAuto);
}

void ofApp::draw_game_over_box(){
    if(loser == 1){
        ofSetColor(255, 0, 0, 127);
    } else {
        ofSetColor(0, 0, 255, 127);
    }
    ofFill();
    ofDrawRectangle(0, 0, ofGetWidth()/2.0, 2*ofGetHeight()/3.0);
    
    if(loser == 1){
        ofSetColor(255, 0, 0);
    } else {
        ofSetColor(0, 0, 255);
    }
    ofSetLineWidth(5);
    ofNoFill();
    ofDrawRectangle(0, 0, ofGetWidth()/2.0, 2*ofGetHeight()/3.0);
}

void ofApp::game_over_message() {
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(ofGetWidth()/4.0, ofGetHeight()/6.0, 0);
    draw_game_over_box();
    ofSetColor(255);
    if (loser == 1) {
        float s_width = hanzipen_50pt.stringWidth("Red Wins!");
        float s_height = hanzipen_50pt.stringHeight("Red Wins!");
        hanzipen_50pt.drawString("Red Wins!", ofGetWidth()/4.0 - s_width/2.0, ofGetHeight()/3.0 - s_height/2.0);
    } else {
        float s_width = hanzipen_50pt.stringWidth("Blue Wins!");
        float s_height = hanzipen_50pt.stringHeight("Blue Wins!");
        hanzipen_50pt.drawString("Blue Wins!", ofGetWidth()/4.0 - s_width/2.0, ofGetHeight()/3.0 - s_height/2.0);
    }
    ofPopMatrix();
    ofPopStyle();
}



//--------------------------------------------------------------
void ofApp::update() {
    if(!game_over) {
        for(int i = 0; i < NUM_PLAYERS; i++) {
            game_over = column_array[i]->update();
            if(game_over){
                loser = i+1;
                break;
            }
        }
        fft_bar.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for(int i = 0; i < NUM_PLAYERS; i++) {
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(((i+1)*ofGetWidth())/4.0, ofGetHeight()/12.0, 0);
        column_array[i]->draw();
        ofPopMatrix();
        ofPopStyle();
    }
    if(game_over) game_over_message();
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(ofGetWidth()/12.0, 9*ofGetHeight()/12.0, 0);
    fft_bar.draw();
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    if(input == NULL && bufferSize == -69 && nChannels == -69) {
        cout << "got here" <<endl;
    } else {
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    for(int i = 0; i < volume_array.size(); i++){
        volume_array[i] = smoothedVol;
    }
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    for(int i = 0; i < NUM_PLAYERS; i++) {
        column_array[i]->resize();
    }
    fft_bar.resize();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    key_state_arr.push_back(key);
}

void ofApp::removeKeyFromState(int key){
    vector <int> new_array;
    for(int i = 0; i < key_state_arr.size(); i++){
        if (key_state_arr[i] != key){
            new_array.push_back(key_state_arr[i]);
        }
    }
    key_state_arr = new_array;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    removeKeyFromState(key);
}



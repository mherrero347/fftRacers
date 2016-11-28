#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofBackground(54, 54, 54);
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    //soundStream.printDeviceList();
    
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    smoothedVol = 0.0;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volume_array.assign(NUM_PLAYERS, 0.0);
    
    for(int i = 0; i < NUM_PLAYERS; i++) {
        column_array.push_back(new PlayerColumn(&volume_array[i], &key_state_arr, i+1));
    }
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update() {
    for(int i = 0; i < NUM_PLAYERS; i++) {
        column_array[i]->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for(int i = 0; i < NUM_PLAYERS; i++) {
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(((i+1)*ofGetWidth())/4.0, ofGetHeight()/6.0, 0);
        column_array[i]->draw();
        ofPopMatrix();
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
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

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    for(int i = 0; i < NUM_PLAYERS; i++) {
        column_array[i]->resize();
    }
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

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}


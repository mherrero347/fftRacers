/* File: ofApp.cpp - created by Matt Herrero
 --------------------------------------------
 * This is the ofApp file, which is the overall controller of the whole game.
 * it creates the player column and fft bar objects, and handles calling their
 * draw and update functions. It also recieves keyboard events and tracks which
 * keys are being held down at each given moment, for use in the PlayerColumn and
 * Racer objects. It also has an audioIn function, which is a sort of psuedo-
 * audio callback (called by the actual audio callback in ofxEasyFFT) and handles
 * calculating the rms volume of each input buffer. It also recognizes and handles
 * game over events.
 */


#include "ofApp.h"
using namespace std::placeholders;

/* --------------------------------------------------------------
                        PUBLIC METHODS
-------------------------------------------------------------- */

/* Destructor: ~ofApp
 -------------------------------
 * This is the destructor of the ofApp class. It frees the memory assigned to 
 * each of the PlayerColumn objects in the column_array vector.
 */
ofApp::~ofApp(){
    column_array.clear();
}

/* Function: setup
 -------------------------------
 * This is the setup function for the ofApp class. Initializes the values of some member
 * variables and object (smoothedVol, game_over, loser, hanzipen_50pt), initializes some
 * member arrays (left, right, volume_array, and isClippingArr), initializes the vector
 * that holds the player columns and allocates memory for those objects, and setups the
 * forward call wrap object so that the audioRecieved method inside the ofxEasyFFT can
 * call this class's audioIn function.
 */
void ofApp::setup(){
    
    ofSetVerticalSync(true);
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
    isClippingArr.assign(NUM_PLAYERS, false);
    
    for(int i = 0; i < NUM_PLAYERS; i++) {
        column_array.push_back(new PlayerColumn(&smoothedVol, &key_state_arr, &isClippingArr, i+1));
    }
    
    auto audioInAuto = std::bind(&ofApp::audioIn, this, _1, _2, _3);
    fft_bar.storeAppAudioCallback(audioInAuto);
}

/* Function: draw_game_over_box
 -------------------------------
 * This function is called when the game ends, and draws the box over the
 * player columns to display the the game over message. It is shaded with
 * the color of the winning racer.
 */
void ofApp::draw_game_over_box(){
    if(loser == 1){
        ofSetColor(255, 0, 0, 127);
    } else {
        ofSetColor(0, 0, 255, 127);
    }
    ofFill();
    ofDrawRectangle(0, 0, 2*ofGetWidth()/3.0, 2*ofGetHeight()/3.0);
    
    if(loser == 1){
        ofSetColor(255, 0, 0);
    } else {
        ofSetColor(0, 0, 255);
    }
    ofSetLineWidth(5);
    ofNoFill();
    ofDrawRectangle(0, 0, 2*ofGetWidth()/3.0, 2*ofGetHeight()/3.0);
}

/* Function: game_over_message
 -------------------------------
 * This function prints the game over message once a player has won.
 */
void ofApp::game_over_message() {
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(ofGetWidth()/6.0, ofGetHeight()/12.0, 0);
    draw_game_over_box();
    ofSetColor(255);
    if (loser == 1) {
        float s_width = hanzipen_50pt.stringWidth("Red Wins!");
        float s_height = hanzipen_50pt.stringHeight("Red Wins!");
        hanzipen_50pt.drawString("Red Wins!", ofGetWidth()/3.0 - s_width/2.0, 5*ofGetHeight()/12.0 - s_height/2.0);
    } else {
        float s_width = hanzipen_50pt.stringWidth("Blue Wins!");
        float s_height = hanzipen_50pt.stringHeight("Blue Wins!");
        hanzipen_50pt.drawString("Blue Wins!", ofGetWidth()/3.0 - s_width/2.0, 5*ofGetHeight()/12.0 - s_height/2.0);
    }
    ofPopMatrix();
    ofPopStyle();
}

/* Function: update
 -------------------------------
 * This is the update function for the ofApp class. It should be called once after
 * each draw call. If the game is not currently over, then the function updates each
 * of the player column object. If either of these column update calls return true, then
 * a collision has occured, and the ending of the game is handled. If not, then the
 * isClippingArr object is updated so it reflects whether each player is currently 
 * clipping their assigned fft bin, and the fft_bar object is updated.
 */
void ofApp::update() {
    if(!game_over) {
        for(int i = 0; i < NUM_PLAYERS; i++) {
            game_over = column_array[i]->update();
            if(game_over){
                loser = i+1;
                break;
            }
            isClippingArr[i] = fft_bar.playerClipping(i+1);
        }
        fft_bar.update();
    }
}

/* Function: draw
 -------------------------------
 * This is the draw function for the ofApp object. It should be called once before
 * every update call. First, it draws each of the player column objects in their current
 * state. Then, it draws the game over message if the game is currently over. Then
 * it draws the fft_bar object in its current state.
 */
void ofApp::draw(){
    for(int i = 0; i < NUM_PLAYERS; i++) {
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(ofGetWidth()/6.0 + i*(ofGetWidth()/3.0), ofGetHeight()/12.0, 0);
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

/* Function: audioIn
 -------------------------------
 * This is the audio "callback" function for the ofApp object. It takes in the current
 * input to the microphone in a vector of floats, along with the bufferSize and
 * number of channels. It find the root mean squared volume across the current input
 * buffer, and stores a smoothed version of this value in the smoothedVol variable.
 
 * NOTE: the reason this is more of an audio "callback" function, and not a true audio
 * callback function is because this isn't actually the function that the audio thread
 * will call everytime a new audio buffer is to be processed. That function is actually
 * the audioReceived function in the ofxEasyFFT class. However, a forward call wrap object
 * of this function has been passed to the ofxEasyFFT class, and that object call this
 * function each time its audio callback fucntion executes. So, functionally, this can
 * be though of as a part of the overall audio callback, which is split between the ofApp
 * class and the ofxEasyFFT class, although technically this is not the audio callback function.
 *
 */
void ofApp::audioIn(vector<float> input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        float start_cur = curVol;
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    curVol /= (float)numCounted;
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
}

/* Function: windowResized
 -------------------------------
 * This is the resize window function, which is called each time the window is resized.
 * if first resizes each of the player columns, then the fft_bar object. It then calls
 * the draw function, which is necessary to avoid a bug in the check_for_collision 
 * function in PlayerColumn, where the function was trying to use new backend values with
 * pre-resize graphical values to detect collisions, and crashing because of it.
 */
void ofApp::windowResized(int w, int h){
    for(int i = 0; i < NUM_PLAYERS; i++) {
        column_array[i]->resize();
    }
    fft_bar.resize();
    draw();
}

/* Function: keyReleased
 -------------------------------
 * This function adds whichever key was just pressed to the key_state_arr vector,
 * which contains the list of all keys currently being pressed down.
 */
void ofApp::keyPressed(int key){
    key_state_arr.push_back(key);
}

/* Function: keyReleased
 -------------------------------
 * This function is passed a key, and removes that key from the key_state_arr vector.
 */
void ofApp::removeKeyFromState(int key){
    vector <int> new_array;
    for(int i = 0; i < key_state_arr.size(); i++){
        if (key_state_arr[i] != key){
            new_array.push_back(key_state_arr[i]);
        }
    }
    key_state_arr = new_array;
}

/* Function: keyReleased
 -------------------------------
 * This function removes a key that has been released from the key_state_arr vector.
 */
void ofApp::keyReleased(int key){
    removeKeyFromState(key);
}



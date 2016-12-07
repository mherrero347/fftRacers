/* File: FftBar.cpp - fftRacers - Created by Matthew Herrero on 12/4/16.
 -----------------------------------------------------------------------
 *
 */

#include "FftBar.h"

/* --------------------------------------------------------------
                        PUBLIC METHODS
 -------------------------------------------------------------- */

/* Constructor: FftBar
 -------------------------------
 * This constructor constructs a new FftBar object. This constructor calculates the
 * dimensions of the containing rectangle, and then initializes some of the fft parameters,
 * including the number of fft frequency bins, making sure the fft is not normalized, the
 * percentage of the full frequency range that being detected (top at about 1000Hz), and the
 * maximum volume. It also sets up the time for the changing of the fft bins and starts that
 * running. Finally, it chooses random bins for each player, and initializes an event
 * listener with a callback function to listen for when the timer has looped back each time,
 * signifying that the players' fft bins should change.
 */
FftBar::FftBar(){
    calc_dimensions();
    fft.setup();
    fft.setNumFFTBins(NUM_FFT_BINS);
    fft.setNormalize(false);
    fft.setFFTpercentage(.047);
    fft.setVolumeRange(400);
    game_bins = NUM_FFT_BINS - TRASH_PREFIX_BINS;
    
    player_bar_timer.setup(3000);
    player_bar_timer.start(true);
    
    changePlayerBins();
    ofAddListener(player_bar_timer.TIMER_COMPLETE, this, &FftBar::changePlayerBinsCallback);
}

/* Function: update
 -------------------------------
 * This is the update function for the Fftbar class. It should be called once after each
 * call to draw. It updates the state of the fft, and then updates the state of the timer.
 */
bool FftBar::update(){
    fft.update();
    player_bar_timer.update();
}

/* Function: getNewPlayerBin
 -------------------------------
 * This function takes in two ints, representing the current bins number of a player, and the
 * bin number of the other player, and returns a new bin number that is different than the
 * old (input) bin number, and different than the opponent's bin.
 */
int FftBar::getNewPlayerBin(int bin, int opponent_bin){
    int old = bin;
    while(true){
        bin = ofRandom(0, game_bins)/1;
        if (bin != old && bin != opponent_bin) break;
    }
    return bin;
}

/* Function: changePlayerBars
 -------------------------------
 * This function reassigns the fft bins for each player to new random bins
 */
void FftBar::changePlayerBins(){
    currBinPlayerOne = getNewPlayerBin(currBinPlayerOne, currBinPlayerTwo);
    currBinPlayerTwo = getNewPlayerBin(currBinPlayerTwo, currBinPlayerOne);
}

/* Function: changePlayerBarsCallback
 -------------------------------
 * This is the callback function that changes the fft bins each player is assigned. It is
 * called each time the player_bar_timer runs out. It calls the changePlayerBins function,
 * which changes the player bins! (*gasp*)
 */
void FftBar::changePlayerBinsCallback(int &i){
    changePlayerBins();
}

/* Function: draw
 -------------------------------
 * This is the draw function for the FftBar class. It should be called once before
 * each update call. It first draws the outer FftBar border, and then calls the
 * draw_bin_bars function to draw the fft bins.
 */
void FftBar::draw(){
    ofPushStyle();
    ofSetColor(245, 58, 135);
    ofNoFill();
    ofDrawRectangle(0, 0, bar_width, bar_height);
    draw_bin_bars();
    ofPopStyle();
}

/* Function: resize
 -------------------------------
 * This is the resize function, which should be called when the window is resized.
 */
void FftBar::resize(){
    calc_dimensions();
}

/* Function: storeAppAudioCallback
 -------------------------------
 * This function is part of the multiple audio callback bug fix. Both ofxEasyFft and 
 * the ofApp classes have audio callback functions that need to execute in order for 
 * the volume walls in the columns and the fft to work at the same time. Therefore
 * a forward call wrap object (the std::function object) is passed down from ofApp,
 * to FftBar, to ofxProcessFFT, to ofxEasyFFT, so that each time the ofxEasyFFT's 
 * audio callback function is called, it can also call the ofApp's audio callback
 * function, and both can live in peace and harmony.
 */
void FftBar::storeAppAudioCallback(std::function<void(vector<float>, int, int)> _audioInApp){
    fft.storeAppAudioCallback(_audioInApp);
}

/* --------------------------------------------------------------
                        PRIVATE METHODS
 -------------------------------------------------------------- */

/* Function: calc_dimensions
 -------------------------------
 * This function calculates the width and height of the FftBar object, based on 
 * predefined ratios relative to the current size of the window.
 */
void FftBar::calc_dimensions() {
    bar_width = 5*ofGetWidth()/6.0;
    bar_height = ofGetHeight()/6.0;
}

/* Function: bin_passed_thresh
 -------------------------------
 * This function takes in an fft bin number and returns true if that bin's
 * volume is at or above the clipping threshold, in which case the gain_multipiler
 * value of the opponent's column should begin to go up.
 */
bool FftBar::bin_passed_thresh(int ind){
    vector<float> spectrum = get_game_spectrum();
    if(spectrum.size()<=0) return false;
    return ofMap(spectrum[ind], 0, fft.getVolumeRange(), 0, 1, true) >= BIN_CLIPPING_THRESH;
}

/* Function: get_game_spectrum
 -------------------------------
 * This function returns the vector of bin amplitudes that will be used for gameplay.
 * This is slightly different than the fft's basic spectrum vector, since we will not
 * use the first TRASH_PREFIX_BINS number of bins in the game (unreachable with the
 * human voice). This function removes those bins and returns the vector.
 */
vector<float> FftBar::get_game_spectrum(){
    vector<float> bins = fft.getSpectrum();
    if(bins.size() > 0) {
        bins.erase(bins.begin(), bins.begin() + TRASH_PREFIX_BINS);
    }
    return bins;
}

/* Function: draw_bin_bars
 -------------------------------
 * This function draws each of the fft bins within the FftBar object. It determines
 * color and shading in the following way: if this bin is currently the blue player's
 * assigned bin, color blue; if it's currently the red player's assigned bin, color red;
 * else, color grey. If the bin is beyond the clipping threshold, fill it with the assigned
 * color, else, leave it hollow. It draws the bins with the height proportional to the 
 * volume of the bin.
 */
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
        ofDrawRectangle(bar_width*((float)i/game_bins), bar_height, bar_width/(game_bins), \
                        -bar_height*ofMap(spectrum[i], 0, fft.getVolumeRange(), 0, 1, true));
    }
    ofPopStyle();
}

/* Function: playerClipping
 -------------------------------
 * This function takes in a player number, and returns true if the bin assigned to
 * that player is clipping at that moment.
 */
bool FftBar::playerClipping(int player){
    if(player == 1) {
        return bin_passed_thresh(currBinPlayerOne);
    } else {
        return bin_passed_thresh(currBinPlayerTwo);
    }
    return false;
}

# fftRacers
fftRacers is a two-player, competitive, scroll-based game built in openFrameworks. Each player controls a dot on the screen, which they can move from left to right. They also control the "walls" of their opponents scrolling column. The objective? Cause your oppenent to crash into the wall, but make sure you don't!

## Game Description
The overall objective of the game is to cause your opponent to crash without crashing yourself. Towards this end, each player has two roles: a racer and a noise-maker.

### Racer ###
As a racer, the player controls their dot (player one on the left side of the screen, player two on the right). They can move the dot from left to right in their column. During the game, the walls of the column will be constantly shrinking towards the center and growing away from the center, based on the microphone input of their opponent. The louder their opponent is, the closer towards the center the walls will shrink! If the racer crashes into one of these walls, the game is over!

**Gain Multiplier:** as a racer, the player can try to build up their "Gain Multiplier." As a player builds up their gain multiplier (starts at x1, then x2, then x3), the walls of their opponent become more sensative to microphone input, leading to larger troughs and peaks, making flying harder for the opponent! To build up this quantity, a racer must spend time in the "danger zone," drawn as the green dotted lines on the right and left of the columns in the drawing below. When the player is in the danger zone, the gain multiplier is increasing. When the player leaves, the gain multiplier begins decreasing. A player's current gain multiplier, as well as their progress toward attaining the next gain multiplier level, is shown in their "Gain Multiplier" progress bar on their control plane (on the left for player 1, on the right for player 2).

### Noise-Maker ###
Each player is also making noise into their own microphone during gameplay. The average volume of the noise they are making determines the shape of their opponent's columns. The louder the player is, the smaller their opponent's scrolling lane becomes! The volume of the incoming noise is stored as time passes. The current volume level is drawn at the top of the opponent's column, and then scrolls down the column over time, creating the shape of the walls. Being quiet might lure the oppenent into the "danger zone," at which point, a loud sound might be enough to cause them to crash! Be careful though! Peaking out the volume of the opponent's column for too long will cause the walls to "overheat." If you've overheated, your opponent's walls won't react to any noise you make while the system cools down, leaving you opponent's danger zone wide open!

**Sweet Spot:** Volume is not the only thing that matters as a noise-maker! The spectrum of the noise you make matters too. If the frequency of the noise you make hits the spectral "Sweet Spot," you can add random positional noise to your opponent's racer, causing them to shake back and forth. The "Sweet Spot" bar in your control plane tells you where the current noise you're making sits on the spectrum (red marker) and where the sweet spot is (yellow bandwidth). The sweet spot is always moving, so try to move with it!

## Display Mock-Up
![fftRacer Mockup](/fftRacersScreenshot.jpg)

This is a sample screenshot of what a moment in the game might look like. The players racing columns are in the center. The scrolling brown walls represent the volume of the microphone input fromt the other player (volume levels toward the top more recent, less recent towards the bottom). The green lines represent the danger zone boundaries for each player (space between that line and the walls is the danger zone). The current gain multiplier for each player is depicted in the "Gain Multiplier" bars to the right and left of the columns (note that both players are currently out of the danger zone, so both multipliers would be decreasing). The "Sweet Spot" bars are below the gain multipliers for each player. The red marker shows where the spectrum is centered for the player's current incoming signal, and the yellow bandwidth shows where the sweet spot currently is. Also, notice that player 1 (left) is currently overheated! That is why the walls of player 2 have receded back to the column edges towards the top.

## Build Instructions
To build this project:
* clone this repository
* open the project in xcode (open fftRacers.xcodeproj)
* run the project in debug mode (hit the play button in the top-left corner of the IED)

Note: the current project is nowhere near an actual, playable version of the game. Full draft coming soon.

Other note: don't know how to create a standalone executable for the current build, or how to build this in a way that it can be opened on Linux/ PC. Help?
 

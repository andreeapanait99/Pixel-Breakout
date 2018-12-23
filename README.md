# Pixel Breakout

A game inspired by Atari Breakout, using an 8x8 matrix and an Arduino UNO board.

## Components
- 8x8 LED matrix
- MAX7219 Driver
- LCD display
- joystick
- 220 ohm resistor
- 3 breadboards of various sizes
- Arduino UNO board

## Game idea
Pixel Breakout is a game inspired by the classic 70s game Atari Breakout. Similar to Atari Breakout, in Pixel Breakout a layer of bricks lines the top third of the matrix. A ball travels across the matrix, bouncing off the top and side walls. When a brick is hit, it is destroyed. The player loses a life when the ball touches the bottom of the matrix. To prevent this from happening, the player has a movable paddle which is controlled by a joystick to bounce the ball upward, keeping it in play.

## Game modes
Both game modes offer 3 lives.

### Play one level mode
The player chooses one of the 5 available levels. In order to win, the player has to finish the aforementioned chosen level without losing all 3 lives. If the player loses one of the lives, the game continues on from where it was left.

### Play all levels mode
The player has to finish all 5 levels in order to win. If the player loses one of the lives, the game restarts from level 1. In case of a loss, at the end of the game the LCD will display the maximum level the player has reached.

## Video

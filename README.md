# ENGR103-Game
Final Project game on the Adafruit Circuit Playground Express for ENGR103
A rhythm game that uses both buttons as inputs, and outputs with the lights and the Serial Monitor.

This game is a rhythm game, with the goal of achieving the highest score possible!
There are 12 rounds of 8 patterns for a total of 96 patterns. Each round is 15% faster than the last, so be careful!
There are 3 possible patterns: a right button press is indicated by red light, a left button press by blue lights, and both simultaneously by green and yellow lights. In every case, the goal is to press on the 8th beat (when the light closest to the corresponding button turns on).

You will be scored depending on your reaction time! 25 ms is considered perfect and awards a score of 100, 50 ms awards 50, 75 ms awards 20, 100 ms awards 5, and anything more than 100 ms is considered a miss. If you press the wrong button (for both buttons, a difference in time of more than 50 ms is considered a miss), you will lose 100 points, so be careful!

As you play, the serial monitor will tell if you are fast or slow.

At the end of the game, you will see your total score out of 9600 and a total rating! Shoot for perfect!

The game is comprised of 12 functions:

void setup: as required by all C++ programs. Initializes the circuit playground and attaches the left and right buttons as interrupts

void loop: Responsible for overall control. Repeats for 12 rounds. First calls on the round function, then increases the game speed, then calls the intermission function. Once all rounds are complete calls, the EndofGame function.

void Intermission: Creates a flashing pattern (red, blue, green, purple) between the rounds to key the player into the new speed before gameplay begins.

void Round: Picks a random number to decide which light pattern the player will play on (left 44%, right 44%, both 12%) and passes off to the corresponding function. Then calls the round scoring function and resets round score to 0.

void score: calculates player reaction time and displays messages depending on whether the players timing was perfect (25 ms, +100 points), good (50 ms, +50 points), okay(75 ms, +20 points), acceptable (100 ms, +5 points), or a miss (100+ ms, 0 points). Also factors in misinputs (-100 points). Prints messages according to these and updates the current and round scores.

void Roundscore: Just updates the roundscores array.

void RBFunction, LBFunction, and BBFunction: Plays the light pattern for the corresponding button press (right and red for RB. left and blue for LB, from both sides with green and yellow for BB), updates the ideal time and actual time for calculating reaction time, and flags wrong button presses. Calls the score function. Finally, truns lights back off.

void EndofGame: prints all game statistics to the serial monitor. Rankings are as follows:
9600           perfect
over 8000      fantastic
over 6000      great
over 4000      good
over 2000      not bad
over 1000      ok
over 0         well... you tried?
less than 0    you should probably try again...

void LBPress and RBPress: Interrupts. They update the last button press time, last left press time, last right press time, and which button was press last boolean, which are used to determine misinputs.

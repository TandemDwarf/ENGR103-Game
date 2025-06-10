# ENGR103-Game
Final Project game on the Adafruit Circuit Playground Express for ENGR103
Intended to be a rhythm game that uses both buttons as inputs, and outputs with the lights and the Serial Monitor.

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

# ENGR103-Game
Final Project game on the Adafruit Circuit Playground Express for ENGR103
Intended to be a rhythm game that uses both buttons as inputs, and outputs with the lights and the Serial Monitor.

The game is comprised of 12 functions:

void setup: as required by all C++ programs. Initializes the circuit playground and attaches the left and right buttons as interrupts

void loop: Responsible for overall control. Repeats for 12 rounds. First calls on the round function, then increases the game speed, then calls the intermission function. Once all rounds are complete calls, the EndofGame function.

void Intermission: Creates a flashing pattern (red, blue, green, purple) between the rounds to key the player into the new speed before gameplay begins.


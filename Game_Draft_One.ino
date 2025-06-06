#include <Adafruit_CircuitPlayground.h>
#include <Math.h>
#include <AsyncDelay.h>

volatile long GlobalTime, IdealPress, ActualPress, ReactionTime;
volatile int CurrentSpeed, CurrentScore = 0, ProbabilityNumber, LastPress, CurrentDelay;  //note: for, lastpress LB is 0, RB is 1.
const int WheelRed = 0, WheelBlue = 150, WheelGreen = 85;
const byte LBpin = 5, RBpin = 4; 

void setup() {  //typical boilerplate setup function
  CircuitPlayground.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(LBpin),LBPress, RISING);  // interrupt for the left button (stop)
  attachInterrupt(digitalPinToInterrupt(RBpin),RBPress, RISING); // interrupt for the right button  (start)
}

void loop() {
  CurrentSpeed = 180;
  CurrentDelay= (60000/CurrentSpeed);
  for(int i=0; i<8; ++i){
      ProbabilityNumber = random(1,16);  //seems to go right like 80% of the time. Not sure why.
      if(ProbabilityNumber<9){
        RBFunction();
      }
      if(ProbabilityNumber>9 && ProbabilityNumber<17){
        LBFunction();
      }
     /* if(ProbabilityNumber>17 && ProbabilityNumber<20){
        BBFunction();
      }
      if(ProbabilityNumber == 20){
        FlipFunction();
      } */
      CurrentScore = (CurrentScore + abs(ReactionTime));
  }
  Serial.println(CurrentScore);
  delay(100000);
}

void RBFunction(){
  IdealPress = (CurrentDelay*8) + millis();  //note: The program currently seems to have the ideal time fall in between LEDs 8 and 9 rather than on 9. I will have to try to fix this later.
    for(int j=0; j<10; ++j){
      for(int i=0; i<(j+1); ++i){
        CircuitPlayground.setBrightness(30);
        CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelRed));
      }
    delay(CurrentDelay);
    }
  delay(8*CurrentDelay);
  ReactionTime = (IdealPress-ActualPress); //negative if early, positive if late.
  if(LastPress != 0){
    Serial.println("WRONG BUTTON!!");
    CurrentScore = CurrentScore+1000;
  }
  //Serial.println(ReactionTime);
  for(int j=0; j<10; ++j){           //turns all light back off.
    CircuitPlayground.setBrightness(0);
    CircuitPlayground.setPixelColor(j, CircuitPlayground.colorWheel(WheelRed));
  }
}

void LBFunction(){
  IdealPress = (CurrentDelay*8) + millis();  //note: The program currently seems to have the ideal time fall in between LEDs 8 and 9 rather than on 9. I will have to try to fix this later.
    for(int j=0; j<10; ++j){
      for(int i=0; i<(j+1); ++i){
        CircuitPlayground.setBrightness(30);
        CircuitPlayground.setPixelColor((10-i), CircuitPlayground.colorWheel(WheelBlue)); //the 10-i is how the direction is flipped, but the 0th led never turns on. Not sure why...
      }
    delay(CurrentDelay);
    }
  delay(8*CurrentDelay);
  ReactionTime = (IdealPress-ActualPress); //negative if early, positive if late.
  if(LastPress != 1){
    Serial.println("WRONG BUTTON!!");
    CurrentScore = CurrentScore+1000;
  }
  //Serial.println(ReactionTime);
  for(int j=0; j<10; ++j){           //turns all light back off.
    CircuitPlayground.setBrightness(0);
    CircuitPlayground.setPixelColor(j, CircuitPlayground.colorWheel(WheelBlue));
  }
}


void LBPress(){
  ActualPress = millis();
  LastPress = 0;
}

void RBPress(){
  ActualPress = millis();
  LastPress = 1;
}
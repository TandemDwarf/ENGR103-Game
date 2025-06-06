#include <Adafruit_CircuitPlayground.h>
#include <Math.h>
#include <AsyncDelay.h>

volatile long GlobalTime, IdealPress, ActualPress, ReactionTime;
volatile int CurrentSpeed, CurrentScore = 0, ProbabilityNumber, LastPress, CurrentDelay;  //note: for, lastpress LB is 0, RB is 1.
const int WheelRed = 0, WheelBlue = 150, WheelGreen = 85;
const byte LBpin = 4, RBpin = 5; 

void setup() {  //typical boilerplate setup function
  CircuitPlayground.begin();
  Serial.begin(9600);
  void LBPress();
  attachInterrupt(digitalPinToInterrupt(LBpin),LBPress, RISING);  // interrupt for the left button (stop)
}

void loop() {
  CurrentSpeed = 180;
  CurrentDelay= (60000/CurrentSpeed);

  IdealPress = (CurrentDelay*8) + millis();  //note: The program currently seems to have the ideal time fall in between LEDs 8 and 9 rather than on 9. I will have to try to fix this later.
    for(int j=0; j<10; ++j){
      for(int i=0; i<(j+1); ++i){
        CircuitPlayground.setBrightness(30);
        CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelBlue));
      }
    delay(CurrentDelay);
    }
  delay(8*CurrentDelay);
  ReactionTime = (IdealPress-ActualPress); //negative if early, positive if late.
  Serial.println(ReactionTime);
  for(int j=0; j<10; ++j){           //turns all light back off.
    CircuitPlayground.setBrightness(0);
    CircuitPlayground.setPixelColor(j, CircuitPlayground.colorWheel(WheelBlue));
  }
}


void LBPress(){
  ActualPress = millis();
  LastPress = 0;
}
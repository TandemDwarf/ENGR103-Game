#include <Adafruit_CircuitPlayground.h>
#include <Math.h>
#include <AsyncDelay.h>

const int WheelBlue = 150;

void setup() {  //typical boilerplate setup function
  CircuitPlayground.begin();
  Serial.begin(9600);
}

void loop() {        //working cirle to the right.
  int CurrentSpeed = 80;
  int CurrentDelay= (60000/CurrentSpeed);

for(int j=0; j<10; ++j){
  for(int i=0; i<(j+1); ++i){
    CircuitPlayground.setPixelColor((10-i), CircuitPlayground.colorWheel(WheelBlue));
  }
delay(250);
}
delay(100000);
}
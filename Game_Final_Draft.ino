/*ENGR103 GAME
By Kahlil Holavarri
No External Code Used!
*/

#include <Adafruit_CircuitPlayground.h>
#include <Math.h>

volatile long GlobalTime, IdealPress, ActualPress, ReactionTime, AbsRTime, RBTime, LBTime; //declaration of all variables and strings used
volatile int CurrentSpeed = 300, CurrentScore = 0, RoundScore = 0, ProbabilityNumber, LastPress, CurrentDelay, CurrentRound, TotalMisses = 0, TotalWrongs = 0;  //note: for lastpress LB is 0, RB is 1.
const int WheelRed = 0, WheelBlue = 150, WheelGreen = 85, WheelYellow = 30, WheelPurple = 210;
const byte LBpin = 5, RBpin = 4; 
const int TDelay = 50;
volatile long RoundScores[12];
volatile bool WrongButton = 0;

void setup() {  //typical boilerplate setup function
  CircuitPlayground.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(LBpin),LBPress, RISING);  // interrupt for the left button press
  attachInterrupt(digitalPinToInterrupt(RBpin),RBPress, RISING); // interrupt for the right button press
}

void loop(){
  for(int h=1; h<13; h++){                  //repeat for 12 rounds
    CurrentRound = h;
    CurrentDelay = (60000/CurrentSpeed);    //Finds delay in ms from speed in bpm
    round();                                //uses the round function to play 1 round of the game
    CurrentSpeed = (1.15*CurrentSpeed);     //increases speed by 15 percent
    Intermission();
  }
  EndofGame();
  delay(999999999999999999999);                                   //terminates program
}

void Intermission(){                            //creates the flashing pattern that plays during intermission
  CircuitPlayground.setBrightness(30);               //turn brightness on
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelRed));     //turn all lights red
  }
  delay(CurrentDelay);                                                            //delay one beat
  CircuitPlayground.setBrightness(0);                                             //turn brightness off
  for(int i=0; i<10; i++){  
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelRed));    //update all light (too off, color doesn't matter)
  }
  delay(CurrentDelay);                                                              //delay one beat
    CircuitPlayground.setBrightness(30);                                            //repeats for the three other colors.
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelBlue));
  }
  delay(CurrentDelay);
  CircuitPlayground.setBrightness(0);
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelRed));
  }
  delay(CurrentDelay);
    CircuitPlayground.setBrightness(30);
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelGreen));
  }
  delay(CurrentDelay);
  CircuitPlayground.setBrightness(0);
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelRed));
  }
  delay(CurrentDelay);
  CircuitPlayground.setBrightness(30);
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelPurple));
  }
  delay(CurrentDelay);
  CircuitPlayground.setBrightness(0);
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelRed));
  }
  delay(CurrentDelay);
}
  
void round() {                              //the round function plays one 8 press round
  for(int i=0; i<8; i++){                   //specifies 8 presses
      ProbabilityNumber = random(1,19);     //random number from 1 to 18 inclusive
      if(ProbabilityNumber<9){              //1 to 8 goes to the RBFunction, 9 to 16 goes to the LBFunction, 17 or 18 goes to the BBFunction
        RBFunction();
      }
      if(ProbabilityNumber>=9 && ProbabilityNumber<17){
        LBFunction();
      }
      if(ProbabilityNumber>=17 && ProbabilityNumber<19){
        BBFunction();
      }
  }
  RoundScoring();                           //goes to the RoundScoring function
  //Serial.println(RoundScore);
  RoundScore = 0;                           //resets RoundScore
}

void Score(){                      //Score Function; called after each press to determine score.
  AbsRTime = abs(ReactionTime);
  if(AbsRTime<=25){                //best tier for scoring; less than or equal to 25 ms is considered perfect and grants a score of 100
    RoundScore = RoundScore+100;
    CurrentScore = CurrentScore+100;
    Serial.println("Perfect!");
  }
    if(AbsRTime>25 && AbsRTime<=50){  //next tier for scoring; less than 50 ms is considered good and grants a score of 50
    RoundScore = RoundScore+50;
    CurrentScore = CurrentScore+50;
    if(ReactionTime<0){
      Serial.println("Early!");       //tells player whether they pressed early or late
    }
    else{
      Serial.println("Late!");
    }
  }
  if(AbsRTime>50 && AbsRTime<=75){  //next tier for scoring; less than 75 ms is considered passable and grants a score of 20
    RoundScore = RoundScore+20;
    CurrentScore = CurrentScore+20;
    if(ReactionTime<0){
      Serial.println("EARLY!");     //tells player whether they pressed early or late
    }
    else{
      Serial.println("LATE!");
    }
  }
  if(AbsRTime>75 && AbsRTime<=100){  //next tier for scoring; less than 100 ms is considered poor and grants a score of 5. Anything worse than 100 ms is considered a miss.
    RoundScore = RoundScore+5;
    CurrentScore = CurrentScore+5;
    if(ReactionTime<0){
      Serial.println("VERY EARLY!");  //tells player whether they pressed early or late
    }
    else{
      Serial.println("VERY LATE!");
    }
  }
  if(AbsRTime>100){
    Serial.println("Miss!");          //if the player was more than 100 ms off, prints a message and does not add to score
    TotalMisses++;
  }

  if(WrongButton == 1){               //if the player pressed the wrong button, prints a message and subtracts 100 from score
    Serial.println("WRONG BUTTON!");
    RoundScore = RoundScore-100;
    CurrentScore = CurrentScore-100;
    TotalWrongs++;
  }
}

void RoundScoring(){                          //the roundscoring function is called at the end of each round. it adds the score for the round to the roundscores array and prints a message to tell the player the next round is starting.
  RoundScores[CurrentRound] = RoundScore;
  Serial.print("Round ");
  Serial.print(CurrentRound);
  Serial.println(" Complete! Speeding up...");
}

void RBFunction(){                   //function for a rightwards button press.
    for(int j=0; j<10; j++){         //nested loop to create light pattern
      if(j==7){
        IdealPress = millis();       //when the 8th light turns on, sets this as the ideal button press time.
      }
      for(int i=0; i<(j+1); i++){
        CircuitPlayground.setBrightness(30);
        CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelRed));
      }
    delay(CurrentDelay);           //delay one beat before repeating.
    }
  delay(8*CurrentDelay);           //delays 8 beats for a late press.
  ReactionTime = (IdealPress-ActualPress); //negative if early, positive if late.
  if(LastPress != 0){              //flags left button presses as incorrect for scoring
    WrongButton = 1;
  }
  Score();                        //calls the Score function
  WrongButton = 0;                //resets wrong button flag
  //Serial.println(ReactionTime);
  for(int j=0; j<10; j++){           //turns all light back off.
    CircuitPlayground.setBrightness(0);
    CircuitPlayground.setPixelColor(j, CircuitPlayground.colorWheel(WheelRed));
  }
}

void LBFunction(){                             //function for a left button press
    for(int j=0; j<10; j++){
      if(j==7){
        IdealPress = millis();       //when the 8th light turns on, sets this as the ideal button press time.
      }
      for(int i=0; i<(j+1); i++){    //nested loop to create light pattern
        CircuitPlayground.setBrightness(30);
        CircuitPlayground.setPixelColor((9-i), CircuitPlayground.colorWheel(WheelBlue)); //the 10-i is how the direction is flipped, but the 0th led never turns on. Not sure why...
      }
    delay(CurrentDelay);             //delays one beat
    }
  delay(8*CurrentDelay);             //delays 8 beats in case of late press
  ReactionTime = (IdealPress-ActualPress); //negative if early, positive if late.
  if(LastPress != 1){                //if the right button was pressed, activates the wrong button flag
    WrongButton = 1;
  }
  Score();                           //calls the score function
  WrongButton = 0;                   //resets the wrong button flag
  //Serial.println(ReactionTime);
  for(int j=0; j<10; j++){           //turns all light back off.
    CircuitPlayground.setBrightness(0);
    CircuitPlayground.setPixelColor(j, CircuitPlayground.colorWheel(WheelBlue));
  }
}

void BBFunction(){                     //function for pressing both buttons
  CircuitPlayground.setBrightness(30);
  for(int j=0; j<5; j++){                 //first half of the light pattern
    for(int i=0; i<(j+1); i++){
      CircuitPlayground.setPixelColor((9-i), CircuitPlayground.colorWheel(WheelGreen));
      CircuitPlayground.setPixelColor((i), CircuitPlayground.colorWheel(WheelGreen));
    }
    delay(CurrentDelay);
  }
  for(int j=5; j<10; j++){
    if(j==7){
        IdealPress = millis();       //when the 8th light turns on, sets this as the ideal button press time.
      }
    for(int i=5; i<(j+1); i++){
      CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(WheelYellow));
      CircuitPlayground.setPixelColor((9-i), CircuitPlayground.colorWheel(WheelYellow));
    }
    delay(CurrentDelay);
  }
  delay(8*CurrentDelay);                //delay 8 beats in case of a late press
  ReactionTime = (IdealPress-ActualPress); //negative if early, positive if late.
  long Difference = abs(LBTime-RBTime);   //checks to see if both buttons were pressed within 50 ms of each other. If not, flags a wrong button press
  if(Difference>50){
    WrongButton = 1;
  }
  Score();                                 //calls the score function
  WrongButton = 0;                         //resets the wrong button flag
  //Serial.println(ReactionTime);
  for(int j=0; j<10; j++){           //turns all light back off.
    CircuitPlayground.setBrightness(0);
    CircuitPlayground.setPixelColor(j, CircuitPlayground.colorWheel(WheelGreen));
  }
}

void EndofGame(){                                          //function that displays end of game stats
  for(int i=0; i<50; i++){ //clears the serial monitor
    Serial.println(" ");
  }
  for(int i=0; i<12; i++){ //prints all round scores
    Serial.print("Round ");
    Serial.print(i+1);
    Serial.print(":   ");
    delay(TDelay);
    Serial.println(RoundScores[i]);
    delay(TDelay);
  }
  Serial.println(" ");                      //displays totals for score, misses, and wrong presses and overall rating
  Serial.println(" ");
  Serial.print("Misses:   ");
  delay(TDelay);
  Serial.print(-TotalMisses);
  delay(TDelay);
  Serial.println(" ");
  Serial.println(" ");
  Serial.print("Misinputs:   ");
  delay(TDelay);
  Serial.print(TotalWrongs);
  delay(TDelay);
  Serial.println(" ");
  Serial.println(" ");
  Serial.print("Overall Score:   ");
  delay(TDelay);
  Serial.print(CurrentScore);
  Serial.print(" / 9600");
  Serial.println(" ");
  Serial.println(" ");
  delay(TDelay);
  Serial.print("Overall Rating:  ");
  delay(TDelay);
  if(CurrentScore == 9600){
    Serial.println("PERFECT GAME!!!");
  }
  if(CurrentScore<9600 && CurrentScore>=8000){
    Serial.println("FANTASTIC!!");
  }
  if(CurrentScore<8000 && CurrentScore>=6000){
    Serial.println("GREAT!");
  }
  if(CurrentScore<6000 && CurrentScore>=4000){
    Serial.println("GOOD!");
  }
  if(CurrentScore<4000 && CurrentScore>=2000){
    Serial.println("NOT BAD");
  }
  if(CurrentScore<2000 && CurrentScore>=1000){
    Serial.println("okay");
  }
  if(CurrentScore<1000 && CurrentScore>=0){
    Serial.println("well...     you tried?");
  }
  if(CurrentScore<0){
    Serial.println("you should probably try again...");
  }
}


void LBPress(){                   //interrupt for a left button press
  ActualPress = millis();         //sets the ActualPress and LBTime to the current time; LBTime is used by the BBFunction only
  LBTime = millis();
  LastPress = 0;                  //updates flag for most recent button press to false (left)
}

void RBPress(){                  //interrupt for a right button press
  ActualPress = millis();        
  RBTime = millis();             //sets the ActualPress and RBTime to the current time; RBTime is used by the BBFunction only
  LastPress = 1;                 //updates flag for most recent button press to true (right)
}
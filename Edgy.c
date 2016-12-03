#include <Servo.h>
#include "Edgy.h"

void setup()
{
  Serial.begin(9600);
  pincer.setup();
  distanceSensor.setup();
  display.setup();
  buzzer.setup();
  drive.setup();
  button.setup();
  bump.setup();
  bump_3.setup();
  
}
// state definitions
const int S = 0;
const int SF = 1; // drive forward slow + arrow
const int B = 2; // drive backward
const int L = 3; // spin left
const int R = 4; // spin right
const int FF = 5; // fast forward + angry face
const int HUG = 6; // close up to the object, hug, smile, release gripper

// initial state
int state = SF;

// flags indicating if there are walls on either side of vehicle
bool leftWall = false;
bool rightWall = false;

// signal to charge ahead
bool charge = false; 

void loop() {

  switch(state) {
   case S:
      drive.stop();
      delay(500); // 0.5 second
      
      if (charge) {
        state = HUG;
      }
      else {
        state = L;
      }
      break;

    case SF:
      drive.forward(128); // half-speed
      // TODO: add random shape: forward arrow, sun or sth that indicates serenity/calmness
      if (distanceSensor.get_distance() < 100) {
        state = S;
      }
      break;

    case B:
      drive.backward(128);
      delay(1000);
      drive.spinRight(128);
      delay(600);
      state = SF; // restart
      break;

    case L:
      drive.spinLeft(128);
      delay(200);
      if (distanceSensor.get_distance() < 100) {
        leftWall = true; 
      }
      state = R;
      break;

    case R:
      drive.spinRight(128);
      delay(400);
      if (distanceSensor.get_distance() < 100) {
        rightWall = true; 
      }
      
      // distance detected is a wall
      if (leftWall & rightWall) {
        spinRight(128);
        delay(400);
        leftWall = false;
        rightWall = false;
        state = SF;
      }
      else {
        charge = true;
        drive.spinLeft(128);
        delay(200);
        state = FF:
      }
      break;

    case FF:
      drive.forward(); // full speed
      // TODO: add angry face >_<
      if (distanceSensor.get_distance() < 20) {
         state = S;
      }
      break;

    case HUG:
      pincer.open();
      delay(200);
   
      drive.forward(128); 
      delay(600); // to fine-tune so that object is in pincher's grab
      
      pincer.close();
      // TODO: add smiling face :)     
      delay(2000);
      
      pincer.open();
      state = B;
      break;
  } // end of switch case

        /**********************
         ORIGINAL TESTER CODE
        ***********************/
//  Serial.println("Testing pincer...");
//  	for(int i = 0; i < 5; i++) {
//  	pincer.open();
//  	delay(1500);
//  	pincer.close();
//  	delay(1500);
//  	}
//    Serial.println("Testing distanceSensor...");
  
  
//  	 for (int c = 0; c < 5000; c++) {
//  	 char str[50];
//  	 sprintf(str, "distanceSensor: distance= %d", distanceSensor.get_distance());
//  	 Serial.println(str);
//  	 delay(1);
//  	}
//        Serial.println("Testing display...");
//  
//  	display.clear();
//  	display.drawCircle(3,3, 3);
//        Serial.println("Testing buzzer...");
//  
//  	buzzer.playNote(NOTE_A4,300);
//  	delay(300);
//  	buzzer.playNote(NOTE_B4,300);
//  	delay(300);
//  	buzzer.playNote(NOTE_C4,300);
//  	delay(300);
//        Serial.println("Testing drive...");
 
//  drive.spinLeft();
//  delay(1000);
//  drive.spinRight();
//  delay(1000);
//        Serial.println("Testing button...");
//  
//  
//  	for (int c = 0; c < 500; c++) {
//  	     char str[50];
//  	     sprintf(str, "button: pressed = %s", button.isPressed() ? "yes" : "no");
//  	     Serial.println(str);
//  	     delay(1);
//  	}
//        Serial.println("Testing bump...");
//  
//  
//  	for (int c = 0; c < 500; c++) {
//  	     char str[50];
//  	     sprintf(str, "bump: pressed = %s", bump.isPressed() ? "yes" : "no");
//  	     Serial.println(str);
//  	     delay(1);
//  	}
//        Serial.println("Testing bump_3...");
//  
//  
//  	for (int c = 0; c < 500; c++) {
//  	     char str[50];
//  	     sprintf(str, "bump_3: pressed = %s", bump_3.isPressed() ? "yes" : "no");
//  	     Serial.println(str);
//  	     delay(1);
//  	}
        
}



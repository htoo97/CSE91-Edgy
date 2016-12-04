#include <Servo.h>
#include "Edgy.h"

/*
 * Note: forward and backward, and left and right are inverted in this code.
 * The arduino robot had to be assembled in such a way that the wheels are inverted.
 */

Song song(buzzer); // "SongData.h" in library contains NyanCat song
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
static const uint8_t PROGMEM
  arrow[] =
  { B00011000,
    B00011000,
    B00011000,
    B10011001,
    B11011011,
    B01111110,
    B00111100,
    B00011000 },

  sadface[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 },
  
  smilyface[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 };

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
  matrix.begin(0x84);
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

void loop()
{
  song.playNextNote();
  
  switch(state) {

   case S:
      drive.stop();
      delay(500); // 0.5 second
      
      if (charge) {
        charge = false;
        state = HUG;   
      }
      else {
        state = L;
      }
      break;


    case SF:
      drive.backward(128); // half-speed
      
      display.clear();
      display.drawBitmap(0, 0, arrow, 8, 8, LED_ON);
      display.writeDisplay();
      
      // distanceSensor's default value drops: it detects changes
      if (distanceSensor.get_distance() < 100) {
        state = S;
      }
      break;


    case B:
      
      drive.forward(128);
      delay(1000);
      drive.spinLeft(128);
      delay(600);
      state = SF; // restart
      break;


    case L:
      drive.spinRight(128);
      delay(200);
      if (distanceSensor.get_distance() < 100) {
        leftWall = true; 
      }
      state = R;
      break;


    case R:
      drive.spinLeft(128);
      delay(400);
      if (distanceSensor.get_distance() < 100) {
        rightWall = true; 
      }
      
      // distance detected is a wall
      if (leftWall & rightWall) {
        drive.spinRight(128);
        delay(200);
        leftWall = false;
        rightWall = false;
        state = SF;
      }
      else {
        charge = true;
        drive.spinRight(128);
        delay(200);
        state = FF;
      }
      break;


    case FF:
      drive.backward(); // full speed
      
      display.clear();
      display.drawBitmap(0,0,sadface,8,8,LED_ON);
      display.writeDisplay();
      
      // until distanceSensor value is close enough
      if (distanceSensor.get_distance() < 20) {
         state = S;
      }
      break;


    case HUG:
      pincer.open();
      delay(1000);
   
      drive.backward(128); 
      delay(600); // fine-tuned so that object is in pincher's grab
      
      display.clear();
      display.drawBitmap(0, 0, smilyface, 8, 8, LED_ON);
      display.writeDisplay();
      pincer.close();
      delay(2000);
            
      pincer.open(); 
      delay(1000);
      state = B;
      break;
  } // end of switch case


}

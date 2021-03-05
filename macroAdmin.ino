/*
  Simple Admin keypad for Squad
  Created by Big Spender#7291
  v2 05/03/2021

*/

/*TODO List
  [X] Deal with initialisation of display
  [X] Set up commands for OBS control
  [X] Only refresh display when mode changes or button pressed
  [X] Individual menu controls not working. Need to handle when mode is the same, but still allow button presses.
  [X] Main menu not needed if we have a mode control as well
  [ ] Swap delays for millis checks

*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);//ox27 is the i2c address of the lcd display. hardware fixed, cannot be changed.

const uint8_t consoleKey = 0x2d; //0x2d is the - key.

const int b1Pin = 8;
const int b2Pin = 7;
const int b3Pin = 6;
const int modePin = 9;

const char adminLockedSquad[] = "AdminBroadcast No locked squads! You can kick who you want, but we do not allow locked squads on our servers.";
const char adminMoveVics[] = "AdminBroadcast Do not move vehicles during staging. It is glitching and you will be kicked for it.";
const char adminWallHeal[] = "AdminBroadcast Do not heal players through walls/floors. It is glitching and you will be kicked for it.";
const char adminSLKit[] = "AdminBroadcast SL's must have an SL kit, a mic, and communicate in command chat in English.";
const char adminCreateSquad[] = "AdminBroadcast Do not create a squad if you do not intend to lead it.";
const char adminVic2Man[] = "All vehicles needing a crewman kit should be manned by at least 2 people.";

const int minDelay = 50;
const int medDelay = 250;
const int maxDelay = 1000;

int b1Pressed = 0;
int b2Pressed = 0;
int b3Pressed = 0;
int bModePressed = 0;

int modeSelect = 1; //default to 1 for initialisation
int prevMode = 0;

const bool keyboardEnable = true;

void setup() {

  lcd.init();
  lcd.backlight();

  pinMode(b1Pin, INPUT_PULLUP);
  pinMode(b2Pin, INPUT_PULLUP);
  pinMode(b3Pin, INPUT_PULLUP);
  pinMode(modePin, INPUT_PULLUP);

  Serial.begin(9600);
  if (keyboardEnable == true) {
    Keyboard.begin();
  }
  else {
    while (!Serial) {
      ;//needed for pro micro
    }
    Serial.println("Serial ready.");
    Serial.print("Mode: ");
    Serial.println(modeSelect);
  }
  displayMenu(modeSelect);
}


void loop() {

  b1Pressed = digitalRead(b1Pin);
  b2Pressed = digitalRead(b2Pin);
  b3Pressed = digitalRead(b3Pin);
  bModePressed = digitalRead(modePin);

  prevMode = modeSelect;

  //check what mode we are in, loop round if reached end of mode menu
  if (bModePressed == LOW) {

    modeSelect ++;
    if (modeSelect > 3) {
      modeSelect = 1;
    }

    Serial.print("Mode: ");
    Serial.println(modeSelect);
    displayMenu(modeSelect);

    delay(medDelay);
  }


  //mode 1 is OBS
  if (modeSelect == 1) {

    if (b1Pressed == LOW) {
      keyboardAction(7);
      delay(medDelay);
      displayMenu(1);
    }
    if (b2Pressed == LOW) {
      keyboardAction(8);
      delay(medDelay);
      displayMenu(1);
    }
    if (b3Pressed == LOW) {
      keyboardAction(9);
      delay(medDelay);
      displayMenu(1);
    }
  }

  //mode 2 is squad admin p1
  if (modeSelect == 2) {

    if (b1Pressed == LOW) {
      keyboardAction(1);
      delay(medDelay);
      displayMenu(2);
    }
    if (b2Pressed == LOW) {
      keyboardAction(2);
      delay(medDelay);
      displayMenu(2);
    }
    if (b3Pressed == LOW) {
      keyboardAction(3);
      delay(medDelay);
      displayMenu(2);
    }
  }


  //mode 3 is squad admin p2
  if (modeSelect == 3) {

    if (b1Pressed == LOW) {
      keyboardAction(4);
      delay(medDelay);
      displayMenu(3);
    }
    if (b2Pressed == LOW) {
      keyboardAction(5);
      delay(medDelay);
      displayMenu(3);
    }
    if (b3Pressed == LOW) {
      keyboardAction(6);
      delay(medDelay);
      displayMenu(3);
    }

  }
}


void displayMenu(int menuSelector) {

  switch (menuSelector) {

    case 1:
      //obs menu
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OBS");
      lcd.setCursor(0, 1);
      lcd.print("1) Start Replay");
      lcd.setCursor(0, 2);
      lcd.print("2) Stop Replay");
      lcd.setCursor(0, 3);
      lcd.print("3) Save Clip");
      break;

    case 2:
      //squad admin 1
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Squad Admin P1");
      lcd.setCursor(0, 1);
      lcd.print("1) Locked Squad");
      lcd.setCursor(0, 2);
      lcd.print("2) Moving Vehicles");
      lcd.setCursor(0, 3);
      lcd.print("3) Squad Creation");
      break;

    case 3:
      //squad admin 2
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Squad Admin P2");
      lcd.setCursor(0, 1);
      lcd.print("1) SL Kit");
      lcd.setCursor(0, 2);
      lcd.print("2) Vehicle 2 Man");
      lcd.setCursor(0, 3);
      lcd.print("3) Healing Walls");
      break;

    default:
      Serial.println("default menu option");
      lcd.clear();
      break;
  }
}


void keyboardAction(int action) {

  switch (action) {

    case 1:
      //locked squad message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Locked Squad");
      if (keyboardEnable == true) {
        Keyboard.write(consoleKey);
        delay(minDelay);
        Keyboard.print(adminLockedSquad);
        delay(minDelay);
        Keyboard.press(KEY_RETURN);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Locked Squad");
      }
      delay(maxDelay);
      break;

    case 2:
      //vehicle move staging
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Moving Vic");
      if (keyboardEnable == true) {
        Keyboard.write(consoleKey);
        delay(minDelay);
        Keyboard.print(adminMoveVics);
        delay(minDelay);
        Keyboard.press(KEY_RETURN);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Moving Vic");
      }
      delay(maxDelay);
      break;

    case 3:
      //squad creation
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Squad Creation");
      if (keyboardEnable == true) {
        Keyboard.write(consoleKey);
        delay(minDelay);
        Keyboard.print(adminCreateSquad);
        delay(minDelay);
        Keyboard.press(KEY_RETURN);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Squad Creation");
      }
      delay(maxDelay);
      break;

    case 4:
      //SL Kit
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SL Kit");
      if (keyboardEnable == true) {
        Keyboard.write(consoleKey);
        delay(minDelay);
        Keyboard.print(adminSLKit);
        delay(minDelay);
        Keyboard.press(KEY_RETURN);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("SL Kit");
      }
      delay(maxDelay);
      break;

    case 5:
      //vehicle 2 man
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vic 2 man");
      if (keyboardEnable == true) {
        Keyboard.write(consoleKey);
        delay(minDelay);
        Keyboard.print(adminVic2Man);
        delay(minDelay);
        Keyboard.press(KEY_RETURN);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Vic 2 man");
      }
      delay(maxDelay);
      break;

    case 6:
      //wall healing
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wall Healing");
      if (keyboardEnable == true) {
        Keyboard.write(consoleKey);
        delay(minDelay);
        Keyboard.print(adminWallHeal);
        delay(minDelay);
        Keyboard.press(KEY_RETURN);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Wall Healing");
      }
      delay(maxDelay);
      break;


    case 7:
      //OBS start replay buffer
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Start Replay");
      if (keyboardEnable == true) {
        Keyboard.press(KEY_F13);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Start Replay");
      }
      delay(maxDelay);

      break;


    case 8:
      //OBS stop replay buffer
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stop Replay");
      if (keyboardEnable == true) {
        Keyboard.press(KEY_F13);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Stop Replay");
      }
      delay(maxDelay);

      break;

    case 9:
      //OBS save replay buffer
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Save Replay");
      if (keyboardEnable == true) {
        Keyboard.press(KEY_F14);
        delay(minDelay);
        Keyboard.releaseAll();
      }
      else {
        Serial.println("Save Replay");
      }
      delay(maxDelay);


      break;
  }
}

#include <IRremote.h>
#include <LiquidCrystal.h>


// This project uses 16x1 screen you might need to adjust it to your screen.

// Pin definitions
const int IR_SEND_PIN = 6;

// LCD pins (rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Joystick pins
const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;
const int JOYSTICK_BTN_PIN = 7;

// Remote control parameters
const int REPEAT_COUNT = 5;
const int DELAY_BETWEEN = 25;
const int BIT_LENGTH = 12;
const int COMMAND_DELAY = 1500;

// Menu navigation
int currentMenuItem = 0;
bool menuNeedsUpdate = true;

// Joystick parameters
const int JOYSTICK_DEADZONE = 100;
const unsigned long JOYSTICK_DELAY = 200;
unsigned long lastJoystickMove = 0;

struct TVCode {
  unsigned long code;
  const char* name;
};

const TVCode SONY_CODES[] = {
  {0xA90, "Power"},
  {0x910, "0"},
  {0x010, "1"},
  {0x810, "2"},
  {0x410, "3"},
  {0xC10, "4"},
  {0x210, "5"},
  {0xA10, "6"},
  {0x610, "7"},
  {0xE10, "8"},
  {0x110, "9"},
  {0x490, "Vol Up"},
  {0xC90, "Vol Dn"},
  {0x090, "Ch Up"},
  {0x890, "Ch Down"},
  {0xA50, "TV/Vid"},
  {0x290, "Mute"},
  {0x070, "Home"},
  {0x190, "PicUp"},
  {0x990, "PicDwn"},
  {0x2D0, "Clear"},
  {0xDD0, "Jump"},
  {0xE90, "MTS"},
  {0x5D0, "Disply"},
  {0xD9A, "Rewind"},
  {0x59A, "Play"},
  {0x99A, "Pause"},
  {0x19A, "Stop"},
  {0xB9A, "Record"},
  {0x39A, "FastFW"},
  {0xCD0, "AM/PM"},
  {0xD10, "Enter"},
  {0x69A, "Eject"},
  {0xC30, "AV1"},
  {0x830, "AV2"},
  {0x430, "AV3"},
  {0xFD0, "TlTxt"},
  {0xA70, "Enter"},
  {0xCD0, "Right"},
  {0x2D0, "Left"},
  {0xAF0, "Down"},
  {0x2F0, "Up"},
  {0x070, "Home"},
  {0xF50, "PwrOFF"},
  {0x750, "PwrON"},
};

const int NUM_CODES = sizeof(SONY_CODES) / sizeof(SONY_CODES[0]);

// Function to pad string to 8 characters
String padString(const char* str, bool addArrow = false) {
  String result = addArrow ? "> " : "  ";
  result += str;
  while (result.length() < 8) {
    result += " ";
  }
  return result.substring(0, 8);
}

void setup() {
  Serial.begin(9600);
  IrSender.begin(IR_SEND_PIN);
  lcd.begin(16, 2);  // or lcd.begin(8, 2)
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);
  
  // Initial display
  updateMenu();
}

void updateMenu() {
  // Clear both halves of the display
  lcd.clear();
  lcd.setCursor(40, 0);
  lcd.print("        ");  // Clear right half
  
  // Display current selection (left half)
  lcd.setCursor(0, 0);
  lcd.print(padString(SONY_CODES[currentMenuItem].name, true));
  
  // Display next item (right half)
  lcd.setCursor(40, 0);
  if (currentMenuItem < NUM_CODES - 1) {
    lcd.print(padString(SONY_CODES[currentMenuItem + 1].name));
  }
}

void sendCode(unsigned long code) {
  for (int i = 0; i < REPEAT_COUNT; i++) {
    IrSender.sendSony(code, BIT_LENGTH);
    delay(DELAY_BETWEEN);
  }
  
  // Visual feedback
  // Might delete this later.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sending ");
  lcd.setCursor(40, 0);
  lcd.print(padString(SONY_CODES[currentMenuItem].name));
  
  delay(1000);
  updateMenu();
}

void handleJoystick() {
  int yValue = analogRead(JOYSTICK_Y_PIN);
  bool buttonPressed = digitalRead(JOYSTICK_BTN_PIN) == LOW;
  
  if (millis() - lastJoystickMove > JOYSTICK_DELAY) {
    if (yValue < 512 - JOYSTICK_DEADZONE) {  // Up
      currentMenuItem = (currentMenuItem > 0) ? currentMenuItem - 1 : NUM_CODES - 1;
      menuNeedsUpdate = true;
      lastJoystickMove = millis();
    } else if (yValue > 512 + JOYSTICK_DEADZONE) {  // Down
      currentMenuItem = (currentMenuItem < NUM_CODES - 1) ? currentMenuItem + 1 : 0;
      menuNeedsUpdate = true;
      lastJoystickMove = millis();
    }
  }
  
  if (buttonPressed) {
    sendCode(SONY_CODES[currentMenuItem].code);
    delay(200);  
  }
}

void loop() {
  handleJoystick();
  
  if (menuNeedsUpdate) {
    updateMenu();
    menuNeedsUpdate = false;
  }
}
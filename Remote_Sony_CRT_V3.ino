#include <IRremote.h>

const int IR_SEND_PIN = 3;

const int REPEAT_COUNT = 5; 
const int DELAY_BETWEEN = 25;
const int BIT_LENGTH = 12;
const int COMMAND_DELAY = 1500; 

bool DEBUG_MODE = false;

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
  {0x490, "Volume Up"},
  {0xC90, "Volume Down"},
  {0x090, "Channel Up"},
  {0x890, "Channel Down"},
  {0x190, "Picture Up"},
  {0x990, "Picture Down"},
  {0xA50, "TV/Video"},
  {0x2D0, "Clear"},
  {0xDD0, "Jump"},
  {0xE90, "MTS"},
  {0x5D0, "Display"},
  {0x290, "Muting"},
  {0xD9A, "Rewind"},
  {0x59A, "Play"},
  {0x99A, "Pause"},
  {0x19A, "Stop"},
  {0xB9A, "Record"},
  {0x39A, "Fast Forward"},
  {0xCD0, "AM/PM"},
  {0xD10, "Enter"},
  {0x69A, "Eject"},
  {0xC30, "AV1"},
  {0x830, "AV2"},
  {0x430, "AV3/COMP1"},
  {0xFD0, "Teletext"},
  {0xA70, "Enter"},
  {0xCD0, "Right"},
  {0x2D0, "Left"},
  {0xAF0, "Down"},
  {0x2F0, "Up"},
  {0x070, "Home"},
  {0xF50, "Power OFF"},
  {0x750, "Power ON"},

};


const int NUM_CODES = sizeof(SONY_CODES) / sizeof(SONY_CODES[0]);

void setup() {
  Serial.begin(9600);
  IrSender.begin(IR_SEND_PIN);
  
  printMenu();
}

void printMenu() {
  Serial.println("\n=== Sony TV Remote Control ===");
  Serial.println("Commands:");
  
  for (int i = 0; i < NUM_CODES; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(SONY_CODES[i].name);
  }
  
  Serial.println("\nSpecial Commands:");
  Serial.println("l: List all codes in hex");
  Serial.println("m: Show this menu");
  Serial.println("d: Toggle debug mode");
  
  Serial.println("\nUsage:");
  Serial.println("- Single command: Just enter the number");
  Serial.println("- Multiple commands: Separate with commas (e.g., 1,18)");
  Serial.println("- Custom hex: Start with 0x (e.g., 0x290)");
}

void listCodes() {
  Serial.println("\nAll codes in hex:");
  for (int i = 0; i < NUM_CODES; i++) {
    Serial.print(SONY_CODES[i].name);
    Serial.print(": 0x");
    Serial.println(SONY_CODES[i].code, HEX);
  }
}


void sendCode(unsigned long code) {
  String codeName = "Custom Code";
  for (int i = 0; i < NUM_CODES; i++) {
    if (SONY_CODES[i].code == code) {
      codeName = SONY_CODES[i].name;
      break;
    }
  }
  
  if (DEBUG_MODE) {
    Serial.print("Sending ");
    Serial.print(codeName);
    Serial.print(" (0x");
    Serial.print(code, HEX);
    Serial.println(")");
  }
  
  for (int i = 0; i < REPEAT_COUNT; i++) {
    IrSender.sendSony(code, BIT_LENGTH);
    delay(DELAY_BETWEEN);
  }
}



void processCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase(); // Case-insensitive commands
  if (cmd.startsWith("0x")) {
    unsigned long customCode = strtoul(cmd.c_str(), NULL, 16);
    sendCode(customCode);
  } else {
    int command = cmd.toInt();
    if (command > 0 && command <= NUM_CODES) {
      sendCode(SONY_CODES[command - 1].code);
    } else {
      Serial.println("Invalid command."); // Handle invalid input
    }
  }
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input == "m") {
      printMenu();
    } else if (input == "l") {
      listCodes();
    } else if (input == "d") {
      DEBUG_MODE = !DEBUG_MODE;
      Serial.print("Debug mode ");
      Serial.println(DEBUG_MODE ? "enabled" : "disabled");
    } else {
      // Handle multiple commands
      int start = 0;
      int comma = input.indexOf(',');
      while (comma != -1 || start < input.length()) {
        String cmd;
        if (comma != -1) {
          cmd = input.substring(start, comma);
          start = comma + 1;
          comma = input.indexOf(',', start);
        } else {
          cmd = input.substring(start);
          start = input.length();
        }
        processCommand(cmd);
        if (start < input.length()) {
          delay(COMMAND_DELAY);
        }
      }
    }
  }
}
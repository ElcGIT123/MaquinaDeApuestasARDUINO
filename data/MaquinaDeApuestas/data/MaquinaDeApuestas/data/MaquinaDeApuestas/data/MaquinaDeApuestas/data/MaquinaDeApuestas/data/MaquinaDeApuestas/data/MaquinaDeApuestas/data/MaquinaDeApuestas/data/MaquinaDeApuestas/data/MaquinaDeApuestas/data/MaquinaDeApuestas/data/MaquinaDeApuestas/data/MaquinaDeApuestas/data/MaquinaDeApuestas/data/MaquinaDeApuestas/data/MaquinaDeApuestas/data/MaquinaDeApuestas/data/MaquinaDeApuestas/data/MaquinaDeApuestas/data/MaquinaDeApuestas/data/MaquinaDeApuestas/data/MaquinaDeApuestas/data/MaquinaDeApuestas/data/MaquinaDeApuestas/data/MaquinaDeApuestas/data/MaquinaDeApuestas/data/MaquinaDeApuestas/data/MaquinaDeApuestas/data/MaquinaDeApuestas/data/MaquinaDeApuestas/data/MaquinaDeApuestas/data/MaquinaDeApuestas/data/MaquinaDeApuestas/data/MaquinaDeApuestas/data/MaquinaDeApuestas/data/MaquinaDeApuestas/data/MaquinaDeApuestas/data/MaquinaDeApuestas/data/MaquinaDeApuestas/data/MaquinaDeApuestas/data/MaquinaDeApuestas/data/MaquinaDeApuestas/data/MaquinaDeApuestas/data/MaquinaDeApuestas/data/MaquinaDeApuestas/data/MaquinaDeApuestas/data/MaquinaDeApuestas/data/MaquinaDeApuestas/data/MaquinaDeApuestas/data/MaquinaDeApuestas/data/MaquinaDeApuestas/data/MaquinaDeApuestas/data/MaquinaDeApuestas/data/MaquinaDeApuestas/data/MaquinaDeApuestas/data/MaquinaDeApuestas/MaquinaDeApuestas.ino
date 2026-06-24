#include <LiquidCrystal.h>

enum MachineStates {
  MENU,
  IN_GAME,
  LOST,
  WIN
}

enum minigames {
  NUMBER_PAR,
  BUILD_NUMBER
}

MachineStates currentState = MENU;
minigames currentGame = NOT_IN_GAME;

// RS, E, D4, D5, D6, D7
LiquidCrystal display(12, 11, 5, 4, 3, 2);

#define CLOCK 7
#define LATCH 9
#define DATA 8

#define JOYSTICK_BUTTON 13
#define JOYSTICK_X A1
#define JOYSTICK_Y A2

byte dataCI1 = 0;
byte dataCI2 = 0;

byte one = 0b00101000;
byte two = 0b11001101;
byte three = 0b01101101;
byte four = 0b00101011;
byte five = 0b01100111;
byte six = 0b11100111;
byte seven = 0b00101101;
byte eight = 0b11101111;
byte nine = 0b00101111;
byte test = 0b00000000;

byte number[9] = {one, two, three, four, five, six, seven, eight, nine};
int byteAsArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  display.begin(16, 2);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);

  display.clear();

  int x = analogRead(JOYSTICK_X);
  int y = analogRead(JOYSTICK_Y);
  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);
}

void loop() {
  switch(state) {
    case MENU:
      initMenu();
    break;
    case IN_GAME:
      setGame(currentGame);
    break;
    case LOST:
      displayLost();
    break
    case WIN: 
      displayWin();
    break;
  }
}

void setGame() {
  switch(currentGame) {
    case NUMBER_PAR:
      initNPGame();
    break;
    case BUILD_NUMBER:
      initBNGame();
    break;
  }
}

void updateCI(byte data, uint8_t ciID) {
  if(ciID == 1) {
      dataCI1 = data;
  } else if(ciID == 2) {
    dataCI2 = data;
  } else {
    Serial.print("Excepcion en updateCI(); Argumento Invalido, no se encontro el id ");
    Serial.print(ciID);
    Serial.print(". Valores validos 1, 2");
    return;
  }

  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI2);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI1);
  digitalWrite(LATCH, HIGH);
}

void resetCI(uint8_t ciID) {
  if(ciID == 1) {
    dataCI1 = 0b00000000;
  } else if(ciID == 2) {
    dataCI2 = 0b00000000;
  } else if(ciID == 3) {
    dataCI1 = 0b00000000;
    dataCI2 = 0b00000000;
  } else {
    Serial.print("Excepcion en resetCI(); Argumento Invalido, no se encontro el id ");
    Serial.print(ciID);
    Serial.print(". Valores validos 1, 2, 3");
    return;
  }
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI2);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI1);
  digitalWrite(LATCH, HIGH);
}

void displayMenuScreen() {
  display.clear();
  display.setCursor(0, 0)
  display.println(" apuesta y gana! ");
  display.print("")
}
void GameNPScreen() {}
void GameNMScreen() {}

void initMenu() {
// phase 1: chose a game
// phase 2: get amount, init


}

void initNPGame() { // number-par game

}

void initBNGame() { //build-number game

}

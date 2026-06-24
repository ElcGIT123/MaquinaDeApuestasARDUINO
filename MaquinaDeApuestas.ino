#include <LiquidCrystal.h>

#include "Config.h"

#include "custom_chars.h"
#include "resources_pack.h"

#include "Game.h"
#include "BetState.h"
#include "Tragamonedas.h"
#include "Blackjack.h"
#include "Buildnumber.h"

MachineStates currentState = MENU;
MachineStates lastState = LOST;
Minigames currentGame = NOT_IN_GAME;
Minigames lastGame = NOT_IN_GAME;

Joystick joystick;

Tragamonedas tragamonedas;
Blackjack blackJack;
Buildnumber buildNumber;

Game* games[] = {
  &tragamonedas,
  &blackJack,
  &buildNumber
};

const size_t GAMES = sizeof(games) / sizeof(games[0]);
uint8_t currentSelectGame = 0;

uint8_t currentMenuScreen = -1;

int globalMoney = 1000;
int betMoney = 100;
int betIncrement = 100;
int betMin = 0;
int betMax = 10000;
BetState bet;

// RS, E, D4, D5, D6, D7
LiquidCrystal display(12, 11, 5, 4, 3, 2);

byte dataCI1 = 0;
byte dataCI2 = 0;

// const byte number[9] = { one, two, three, four, five, six, seven, eight, nine };

void setup() {
  Serial.begin(9600);
  display.begin(16, 2);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);

  resetCI(3);
  display.clear();

  pinMode(13, INPUT_PULLUP);
}

void loop() {
  joystick.update();

  if (currentState != lastState) {
    switch (currentState) {
      case MENU:
        initMenuScreen();
        break;
      case CONFIG:
        initConfigScreen();
        break;
      case TUTORIAL:
        initTutorialScreen();
        break;
      case IN_GAME:
        initGame();
        break;
      /* case LOST:
        initLost();
        break;
      case WIN:
        initWin();
        break; */
    }
    lastState = currentState;
  }

  switch (currentState) {
    case MENU:
      updateChannels();
      break;
    case CONFIG:
      updateChannels();
      break;
    case TUTORIAL:
      updateChannels();
      break;
    case IN_GAME:
      updateGame();
      break;
    /* case LOST:
      updateLost();
      break;
    case WIN:
      updateWin();
      break; */
  }
}

void initGame() {
  games[currentSelectGame]->init(display);
}

void updateGame() {
  games[currentSelectGame]->update(display, joystick);
}

void updateCI(byte data, uint8_t ciID) {
  if (ciID == 1) {
    dataCI1 = data;
  } else if (ciID == 2) {
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
  if (ciID == 1) {
    dataCI1 = 0b00000000;
  } else if (ciID == 2) {
    dataCI2 = 0b00000000;
  } else if (ciID == 3) {
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

void uploadResourcePack(byte* icons[], uint8_t amount) {
  for(uint8_t i = 0; i < amount; i++) {
    display.createChar(i, (uint8_t*)icons[i]);
  }
} 

// MENU
void initMenuScreen() {
  // draw
  display.clear();
  uploadResourcePack(
    menuSrcPack, 
    sizeof(menuSrcPack) / sizeof(menuSrcPack[0])
  );

  display.setCursor(1, 0);
  display.write(byte(0));
  display.print(" MINIJUEGOS ");
  display.write(byte(0));

  display.setCursor(0, 1);
  display.write(byte(1));
  display.write(byte(4));
  display.write(byte(2));

  display.setCursor(13, 1);
  display.write(byte(1));
  display.write(byte(3));
  display.write(byte(2));

  display.setCursor(6, 1);
  display.print("menu");
}

void updateMenu() {
  // y
  if (joystick.up()) {
    currentState = TUTORIAL;
    Serial.println("Canal tutorial");
    delay(500);
    return;
  }
  if (joystick.down()) {
    currentState = CONFIG;
    Serial.print("Canal CONFIG");
    delay(500);
    return;
  }
  // X
  if (joystick.right()) {
    Serial.println("RECORRIENDO DERECHA TEST");
    currentMenuScreen++;
    if (currentMenuScreen > GAMES) {
      currentMenuScreen = -1;
    }
    delay(400);
    if(currentMenuScreen == -1) {
      initMenuScreen();
      return;
    }
    games[currentMenuScreen]->initScreen(display);
    return;
  }
  if (joystick.left()) {
    Serial.println("RECORRIENDO IZQUIERDA TEST");
    currentMenuScreen--;
    if (currentMenuScreen < -1) {
      currentMenuScreen = GAMES - 1;
    }
    delay(400);
    if(currentMenuScreen == -1) {
      initMenuScreen();
      return;
    }
    games[currentMenuScreen]->initScreen(display);
    return;
  }
  // button
  if (joystick.pressed()) {
    if (currentMenuScreen == -1) {
      Serial.print("MENU");
      delay(500);
      return;
    }
    Serial.println("OTRO");
    currentState = IN_GAME;
    currentSelectGame = currentMenuScreen;
    currentMenuScreen = -1;
    delay(500);
  }

}

void initConfigScreen() {
  display.clear();
  display.setCursor(0, 0);
  display.print("CONFIG");
}

void updateConfig() {
  if (joystick.up()) {
    currentState = MENU;
    Serial.println("channel: MENU");
    delay(300);
    return;
  }
  if (joystick.down()) {
    currentState = TUTORIAL;
    Serial.println("channel: TUTORIAL");
    delay(300);
    return;
  }
  if (joystick.pressed()) {
    Serial.println("IN CONFIG");
    delay(300);
  }
}

void initTutorialScreen() {
  display.clear();
  display.setCursor(0, 0);
  display.print("TUTORIAL");

  Serial.print("INIT TUTORIAL TEST");
}

void updateTutorial() {
  if (joystick.up()) {
    currentState = CONFIG;
    Serial.println("channel: CONFIG");
    delay(500);
    return;
  }
  if (joystick.down()) {
    currentState = MENU;
    Serial.println("channel: MENU");
    delay(500);
    return;
  }
  if (joystick.pressed()) {
    Serial.println("TEST TUTORIAL");
    delay(300);
  }
}

void updateChannels() {
  switch (currentState) {
    case MENU:
      updateMenu();
    break;
    case CONFIG:
      updateConfig();
    break;
    case TUTORIAL:
      updateTutorial();
    break;
  }
}

String centerText(String text) {
  if(text.length() > 16) {
    return "IncompatibleText";
  }
  uint8_t spaces = (16 - text.length()) / 2;
  String centeredText = "";
  for(int i = 0; i < spaces; i++) {
    centeredText += " ";
  }
  centeredText += text;
  return centeredText;
}

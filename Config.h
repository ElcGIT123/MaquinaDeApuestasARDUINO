#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define CLOCK 7
#define LATCH 9
#define DATA 8

class Game;
class BetState;

enum MachineStates {
  MENU,
  CONFIG,
  TUTORIAL,
  IN_GAME,
  LOST,
  WIN
};

enum Minigames {
  NOT_IN_GAME,
  NUMBER_PAR,
  BUILD_NUMBER
};

struct Joystick {
  int x;
  int y;
  int button;

  void update() {
    x = analogRead(A1);
    y = analogRead(A2);
    button = digitalRead(13);
  }

  bool right() { return x <= 300; }
  bool left() { return x >= 800; }
  bool up() { return y <= 200; }
  bool down() { return y >= 900; }
  bool pressed() { return button == LOW; }
};

extern MachineStates currentState;
extern Minigames currentGame;
extern uint8_t currentMenuScreen;

extern int globalMoney;
extern int betMoney;
extern int betIncrement;
extern int betMin;
extern int betMax;

extern BetState bet;

#endif
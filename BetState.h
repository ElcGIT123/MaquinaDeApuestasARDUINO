#ifndef BETSTATE_H
#define BETSTATE_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Game.h"
#include "custom_chars.h"

#include "Config.h"

void initGame();

const byte* const bsResourcePack[] PROGMEM = {
  FRAME_RIGHT,
  FRAME_LEFT,
  DOLLAR_SIMBOL
};

class BetState : public Game {

  public:
    bool isDeclared;
    uint8_t increment;
    BetState() : Game(
      "apuesta", 
      (const byte**)bsResourcePack, 
      sizeof(bsResourcePack) / sizeof(bsResourcePack[0]),
      betMoney
    ),
    isDeclared(false),
    increment(betIncrement)
  {}

  void initScreen(LiquidCrystal &display) override {
    // unusable
  }

  void init(LiquidCrystal &display) override {
    uploadGameResourcePack(display);
    display.clear();
    display.setCursor(0, 5);
    display.print("APUESTA");

    display.setCursor(1, 1);
    display.print(betMin);
    display.setCursor(5, 1);
    display.write(byte(0));
    display.write(byte(2));
    display.print(betMoney);
    display.setCursor(9, 1);
    display.write(byte(1));
    display.setCursor(11, 1);
  }

  void update(LiquidCrystal &display, Joystick &joystick) override {
    if (joystick.pressed()) {
      this->isDeclared = true;
      initGame();
      delay(300);
      return;
    }
    Serial.print("BSTATE");
    delay(1000);
  }

};

#endif
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

  private:
    unsigned long timeBS;
    unsigned long timeButton;

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
    increment(betIncrement),
    timeBS(0),
    timeButton(0)
  {}

  void initScreen(LiquidCrystal &display) override {
    // unusable
  }

  void init(LiquidCrystal &display) override {
    timeBS = 0;
    timeButton = 0;

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
    unsigned long currentTime = millis();

    if (joystick.pressed()) {
      this->isDeclared = true;
      if(currentTime - timeButton >= 300) {
        timeButton = currentTime;
        initGame();
        return;
      }
    }
    if(currentTime - timeBS >= 1000) {
      timeBS = currentTime;
      Serial.print("BSTATE");
    }
  }

};

#endif
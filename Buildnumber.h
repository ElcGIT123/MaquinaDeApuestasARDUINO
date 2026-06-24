#ifndef BUILDNUMBER_H
#define BUILDNUMBER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Game.h"
#include "custom_chars.h"

#include "Config.h"

const byte* const bnResourcePack[] PROGMEM = {
  FRAME_RIGHT,
  FRAME_LEFT
};

class Buildnumber : public Game {

  private:
    int playPrice;
    bool isPlay;

  public:
    Buildnumber() : Game(
      "BuildNumber", 
      (const byte**)bnResourcePack, 
      sizeof(bnResourcePack) / sizeof(bnResourcePack[0]),
      betMoney
    ),
    playPrice(1000),
    isPlay(true)
  {}

  void initScreen(LiquidCrystal &display) override {
    uploadGameResourcePack(display);
    
    display.setCursor(0, 0);
    display.clear();
    gameScreenTemplate(display, this->name);
  }

  void init(LiquidCrystal &display) override {
    uploadGameResourcePack(display);
    display.clear();

    display.setCursor(0, 0);
    display.print("BUildNumber init");
  }

  void update(LiquidCrystal &display, Joystick &joystick) override {
    Serial.print("BM");
    delay(1000);
    if (joystick.pressed()) {
      Serial.print(" \n BUILDNUMBER PASS");
      delay(1000);
      currentState = MENU;
      currentGame = NOT_IN_GAME;
    }
  }

};

#endif
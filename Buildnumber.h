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
    unsigned long timeBN;
    unsigned long timeButton;

  public:
    Buildnumber() : Game(
      "BuildNumber", 
      (const byte**)bnResourcePack, 
      sizeof(bnResourcePack) / sizeof(bnResourcePack[0]),
      betMoney
    ),
    playPrice(1000),
    isPlay(true),
    timeBN(0),
    timeButton(0)
  {}

  void initScreen(LiquidCrystal &display) override {
    uploadGameResourcePack(display);
    
    display.setCursor(0, 0);
    display.clear();
    gameScreenTemplate(display, this->name);
  }

  void init(LiquidCrystal &display) override {
    timeBN = millis();
    timeButton = millis();

    uploadGameResourcePack(display);
    display.clear();

    display.setCursor(0, 0);
    display.print("BUildNumber init");
  }

  void update(LiquidCrystal &display, Joystick &joystick) override {
    unsigned long currentTime = millis();
    
    if(currentTime - timeBN >= 1000){
      timeBN = currentTime;
      Serial.print("BM");
    }
    
    if (joystick.pressed()) {
      if(currentTime - timeButton >= 300) {
        timeButton = currentTime;
        Serial.print(" \n BUILDNUMBER PASS");
        currentState = MENU;
        currentGame = NOT_IN_GAME;
      }
    }
  }

};

#endif
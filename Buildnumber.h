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
      2,
      betMoney
    ),
    playPrice(1000),
    isPlay(true),
    timeBN(0),
    timeButton(0)
  {}

  void initScreen(LiquidCrystal &display) override {
    display.clear();
    uploadGameResourcePack(display);
    
    display.setCursor(0, 0);
    gameScreenTemplate(display, this->name);
    display.setCursor(0, 1);
    display.write(byte(0));
    display.write(byte(1));
  }

  void init(LiquidCrystal &display) override {
    timeBN = millis();
    timeButton = millis();

    display.clear();
    uploadGameResourcePack(display);

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
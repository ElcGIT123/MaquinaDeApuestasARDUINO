#ifndef TMGAME_H
#define TMGAME_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Game.h"
#include "custom_chars.h"

#include "Config.h"

const byte* const bjResourcePack[] PROGMEM = {
  FRAME_RIGHT,
  FRAME_LEFT,
};

class Blackjack : public Game {

  private:
    int playPrice;
    bool isPlay;
    unsigned long timeBJ;
    unsigned long timeButton;

  public:
    Blackjack() : Game(
      "BlackJack", 
      (const byte**)bjResourcePack, 
      2,
      betMoney
    ),
    playPrice(1000),
    isPlay(true),
    timeBJ(0),
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
    timeBJ = millis();
    timeButton = millis();

    display.clear();
    uploadGameResourcePack(display);

    display.setCursor(0, 0);
    display.print("Blackjack init 7");
  }

  void update(LiquidCrystal &display, Joystick &joystick) override {
    unsigned long currentTime = millis();

    if(currentTime - timeBJ >= 1000) {
      timeBJ = currentTime;
      Serial.print("BJ");
    }

    if (joystick.pressed()) {
      if(currentTime - timeButton >= 300) {
        timeButton = currentTime;
        Serial.print(" \n BLACKJACK PASSED");
        currentState = MENU;
        currentGame = NOT_IN_GAME;
      }
    }
  }

};

#endif
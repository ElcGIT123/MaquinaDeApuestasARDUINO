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

  public:
    Blackjack() : Game(
      "BlackJack", 
      (const byte**)bjResourcePack, 
      sizeof(bjResourcePack) / sizeof(bjResourcePack[0]),
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
    display.print("Blackjack init 7");
  }

  void update(LiquidCrystal &display, Joystick &joystick) override {
    Serial.print("BJ");
    delay(1000);
    if (joystick.pressed()) {
      Serial.print(" \n BLACKJACK PASSED");
      delay(1000);
      currentState = MENU;
      currentGame = NOT_IN_GAME;
    }
  }

};

#endif
#ifndef TRAGAMONEDAS_H
#define TRAGAMONEDAS_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Game.h"
#include "custom_chars.h"

#include "BetState.h"

#include "Config.h"

const byte* const tmResourcePack[] PROGMEM = {
  FRAME_RIGHT,
  FRAME_LEFT,
  HEART,
  DIAMOND,
  BELL,
  DOLLAR_SIMBOL,
  CHERRY
};

class Tragamonedas : public Game {

  private:
    int playPrice;
    bool isPlay;
    unsigned long timeTM;
    unsigned long timeButton;

  public:
    Tragamonedas()
    : Game(
      "Tragamonedas", 
      (const byte**)tmResourcePack, 
      sizeof(tmResourcePack) / sizeof(tmResourcePack[0]),
      betMoney
    ),
    playPrice(1000),
    isPlay(true),
    timeTM(0),
    timeButton(1000)
  {}

  void initScreen(LiquidCrystal &display) override {
    uploadGameResourcePack(display);

    display.clear();
    gameScreenTemplate(display, this->name);

    display.setCursor(2, 1);
    display.write(byte(0));
    display.write(byte(2));
    display.write(byte(1));
    display.print(" ");
    display.write(byte(0));
    display.write(byte(3));
    display.write(byte(1));
    display.print(" ");
    display.write(byte(0));
    display.write(byte(4));
    display.write(byte(1));
  }

  void init(LiquidCrystal &display) override {
    timeTM = millis();
    timeButton = millis();

    if(!bet.isDeclared) { 
      bet.init(display);
      return; 
    }

    uploadGameResourcePack(display);
    display.clear();

    display.setCursor(2, 1);
    display.write(byte(0));
    display.write(byte(4));
    display.write(byte(1));
    display.print(" ");
    display.write(byte(0));
    display.write(byte(4));
    display.write(byte(1));
    display.print(" ");
    display.write(byte(0));
    display.write(byte(4));
    display.write(byte(1));
  }

  void update(LiquidCrystal &display, Joystick &joystick) override {
    unsigned long currentTime = millis();

    if(!bet.isDeclared) { 
      bet.update(display, joystick); 
      return; 
    }

    if(currentTime - timeTM >= 1000){
      timeTM = currentTime;
      Serial.print("TM");
    }

    if (joystick.pressed()) {
      if(currentTime - timeButton >= 300){
        Serial.print(" \n TEST PASSED");
        bet.isDeclared = false;
        currentState = MENU;
        currentGame = NOT_IN_GAME;
      }
    }
  }

};

#endif
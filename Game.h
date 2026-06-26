#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Config.h"

extern int betMoney;
extern int globalMoney;

enum MachineStates;
enum Minigames;

extern MachineStates currentState;
extern Minigames currentGame;

extern int8_t currentMenuScreen;

String centerText(String text);

struct Joystick;
extern Joystick joystick;

class Game;
extern Game* games[];

class Game {
  protected:
    String name;
    const byte** resourcePack;
    uint8_t packSize;
    int betMoney;

  public:
    Game(String name, const byte** resourcePack, uint8_t packSize, int betMoney)
    : name(name), resourcePack(resourcePack), packSize(packSize), betMoney(betMoney) {}

  void uploadGameResourcePack(LiquidCrystal &display) {
      for(uint8_t i = 0; i < packSize; i++) {
        // pgm_read_ptr es necesario para leer las direcciones desde la memoria Flash
        byte* iconAddress = (byte*)pgm_read_ptr(&(resourcePack[i]));
        display.createChar(i, (uint8_t*)iconAddress);
      }
    }

  void gameScreenTemplate(
    LiquidCrystal &display, String title, 
    bool icon = false, byte iconByte = 0) {
    display.print(centerText(title));
    if(icon) {
      display.write(byte(iconByte));
    }
  } 

    virtual void initScreen(LiquidCrystal &display) = 0;
    virtual void init(LiquidCrystal &display) = 0;
    virtual void update(LiquidCrystal &display, Joystick &joystick) = 0;

    virtual ~Game() {}
};

#endif
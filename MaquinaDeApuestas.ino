/**
* @file MaquinaDeApuestas.ino
* @author ELC/ELchinomandaron
* @brief Sistema de maquina de apuestas basada en FSM(Finite State Machine) y un Game loop
* @version 0.2.0-poc 
* @date 2026-06-25
**/

#include <LiquidCrystal.h>

#include "Config.h"

#include "custom_chars.h"
#include "resources_pack.h"

#include "Game.h"
#include "BetState.h"
#include "Tragamonedas.h"
#include "Blackjack.h"
#include "Buildnumber.h"

MachineStates currentState; ///< Estado actual de la FSM
MachineStates lastState;    ///< Ultimo estado de la FSM

Minigames currentGame;      ///< Juego actual

Joystick joystick;          ///< Struct que maneja el Joystick analogico HW-504

/// instancias de los juegos disponibles 
Tragamonedas tragamonedas;
Blackjack blackJack;
Buildnumber buildNumber;

Game* games[] = {
  &tragamonedas,
  &blackJack,
  &buildNumber
};

const size_t GAMES = sizeof(games) / sizeof(games[0]); ///< Cardinalidad de games[]
uint8_t currentSelectGame;    ///< Juego actual seleccionado

uint8_t currentMenuScreen;    ///< Pantalla actual en el menu

int globalMoney; 

int betMoney;     ///< Dinero de apuesta
int betIncrement; ///< Incremento de la apuesta
int betMin;       ///< Minima apuesta
int betMax;       ///< Maxima apuesta

BetState bet;     ///< Clase que controla el estado de mannipular la apuesta

// RS, E, D4, D5, D6, D7
LiquidCrystal display(12, 11, 5, 4, 3, 2);

byte dataCI1 = 0; ///< Bits en el primer CI SN74HC595
byte dataCI2 = 0; ///< Bits en el segundo CI SN74HC595

/**
* @brief Configuracion inicial del Hardware y del monitor Serial
* ademas inicializa los valores de la maquina
*/
void setup() {
  Serial.begin(9600);
  display.begin(16, 2);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);

  resetCI(3);
  display.clear();

  pinMode(13, INPUT_PULLUP);

  currentState = MENU;
  lastState = LOST;

  currentGame = NOT_IN_GAME;
  currentSelectGame = 0;

  currentMenuScreen = -1;

  globalMoney = 1000; 

  betMoney = 100;
  betIncrement = 100;
  betMin = 0; 
  betMax = 10000; 
}

/**
* @brief Gestiona el ciclo de vida de la maquina. Se encarga de actualizar el valor del input del joystick y 
* el update() e init() de los estados de la FSM 
*/
void loop() {
  joystick.update();

  if (currentState != lastState) {
    switch (currentState) {
      case MENU:
        initMenuScreen();
        break;
      case CONFIG:
        initConfigScreen();
        break;
      case TUTORIAL:
        initTutorialScreen();
        break;
      case IN_GAME:
        initGame();
        break;
    }
    lastState = currentState;
  }

  switch (currentState) {
    case MENU:
      updateChannels();
      break;
    case CONFIG:
      updateChannels();
      break;
    case TUTORIAL:
      updateChannels();
      break;
    case IN_GAME:
      updateGame();
      break;
  }
}

/**
* @brief Inicializa el juego corresponidente segun el valor en currentSelectGame
*/
void initGame() {
  games[currentSelectGame]->init(display);
}

/**
* @brief Actualiza el juego correspondiente segun el valor en currentSelectGame
*/
void updateGame() {
  games[currentSelectGame]->update(display, joystick);
}

/**
* @brief Actualiza el registro de 8 bits del CI 74HC595
*
* @param data Es el nuevo valor de 8 bits que tomara el CI 74HC595
* @param ciID Es el CI que se actualizara con el valor de data, 1 hace referencia al primero y 2 hace referencia
* el CI que recibe el pin Q7´ 
**/
void updateCI(byte data, uint8_t ciID) {
  if (ciID == 1) {
    dataCI1 = data;
  } else if (ciID == 2) {
    dataCI2 = data;
  } else {
    Serial.print("Excepcion en updateCI(); Argumento Invalido, no se encontro el id ");
    Serial.print(ciID);
    Serial.print(". Valores validos 1, 2");
    return;
  }

  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI2);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI1);
  digitalWrite(LATCH, HIGH);
}

/**
* @brief Borra el registro de 8 bits del CI 74HC595
*
* @param ciID es el CI que se borara su registro, 1 hace referencia al primero y 2 hace referencia
* el CI que recibe el pin Q7´ y 3 hace referencia a los dos
**/
void resetCI(uint8_t ciID) {
  if (ciID == 1) {
    dataCI1 = 0b00000000;
  } else if (ciID == 2) {
    dataCI2 = 0b00000000;
  } else if (ciID == 3) {
    dataCI1 = 0b00000000;
    dataCI2 = 0b00000000;
  } else {
    Serial.print("Excepcion en resetCI(); Argumento Invalido, no se encontro el id ");
    Serial.print(ciID);
    Serial.print(". Valores validos 1, 2, 3");
    return;
  }
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI2);
  shiftOut(DATA, CLOCK, LSBFIRST, dataCI1);
  digitalWrite(LATCH, HIGH);
}

/**
* @brief Crea caracteres personalizados para el LCD 
*
* @param icons es un array que contiene a la vez los arrays de los iconos personalizados que se agregaran a la pantalla LCD
* @param amount se pide la cardinalidad del bucle for para saber cuando parar de usar LiquidCrystal.createChar
*
*/
void uploadResourcePack(byte* icons[], uint8_t amount) {
  for(uint8_t i = 0; i < amount; i++) {
    display.createChar(i, (uint8_t*)icons[i]);
  }
} 

/**
* @brief Inicializa en el display la pantalla de Menu
*/
void initMenuScreen() {
  // draw
  display.clear();
  uploadResourcePack(
    menuSrcPack, 
    sizeof(menuSrcPack) / sizeof(menuSrcPack[0])
  );

  display.setCursor(1, 0);
  display.write(byte(0));
  display.print(" MINIJUEGOS ");
  display.write(byte(0));

  display.setCursor(0, 1);
  display.write(byte(1));
  display.write(byte(4));
  display.write(byte(2));

  display.setCursor(13, 1);
  display.write(byte(1));
  display.write(byte(3));
  display.write(byte(2));

  display.setCursor(6, 1);
  display.print("menu");
}

/**
* @brief Actualiza la logica del menu
*/
void updateMenu() {
  // y
  if (joystick.up()) {
    currentState = TUTORIAL;
    Serial.println("Canal tutorial");
    delay(500);
    return;
  }
  if (joystick.down()) {
    currentState = CONFIG;
    Serial.print("Canal CONFIG");
    delay(500);
    return;
  }
  // X
  if (joystick.right()) {
    Serial.println("RECORRIENDO DERECHA TEST");
    currentMenuScreen++;
    if (currentMenuScreen > GAMES) {
      currentMenuScreen = -1;
    }
    delay(400);
    if(currentMenuScreen == -1) {
      initMenuScreen();
      return;
    }
    games[currentMenuScreen]->initScreen(display);
    return;
  }
  if (joystick.left()) {
    Serial.println("RECORRIENDO IZQUIERDA TEST");
    currentMenuScreen--;
    if (currentMenuScreen < -1) {
      currentMenuScreen = GAMES - 1;
    }
    delay(400);
    if(currentMenuScreen == -1) {
      initMenuScreen();
      return;
    }
    games[currentMenuScreen]->initScreen(display);
    return;
  }
  // button
  if (joystick.pressed()) {
    if (currentMenuScreen == -1) {
      Serial.print("MENU");
      delay(500);
      return;
    }
    Serial.println("OTRO");
    currentState = IN_GAME;
    currentSelectGame = currentMenuScreen;
    currentMenuScreen = -1;
    delay(500);
  }

}

/**
* @brief Inicializa en el display la pantalla de Config
*/
void initConfigScreen() {
  display.clear();
  display.setCursor(0, 0);
  display.print("CONFIG");
}

/**
* @brief Actualiza la logica de Config
*/
void updateConfig() {
  if (joystick.up()) {
    currentState = MENU;
    Serial.println("channel: MENU");
    delay(300);
    return;
  }
  if (joystick.down()) {
    currentState = TUTORIAL;
    Serial.println("channel: TUTORIAL");
    delay(300);
    return;
  }
  if (joystick.pressed()) {
    Serial.println("IN CONFIG");
    delay(300);
  }
}

/**
* @brief Inicializa en el display la pantalla de Tutorial
*/
void initTutorialScreen() {
  display.clear();
  display.setCursor(0, 0);
  display.print("TUTORIAL");

  Serial.print("INIT TUTORIAL TEST");
}

/**
* @brief Actualiza la logica de Tutorial
*/
void updateTutorial() {
  if (joystick.up()) {
    currentState = CONFIG;
    Serial.println("channel: CONFIG");
    delay(500);
    return;
  }
  if (joystick.down()) {
    currentState = MENU;
    Serial.println("channel: MENU");
    delay(500);
    return;
  }
  if (joystick.pressed()) {
    Serial.println("TEST TUTORIAL");
    delay(300);
  }
}

/**
* @brief Enrutador que simplifica la actualizacion de los canales
*/
void updateChannels() {
  switch (currentState) {
    case MENU:
      updateMenu();
    break;
    case CONFIG:
      updateConfig();
    break;
    case TUTORIAL:
      updateTutorial();
    break;
  }
}

/**
* @brief Centra un texto entre 16 espacios, para centrar textos en la pantalla LCD
*
* @param text el texto que se retornara centrado entre 16 espacios
* @return String, la variable text centrada
*/
String centerText(String text) {
  if(text.length() > 16) {
    return "IncompatibleText";
  }
  uint8_t spaces = (16 - text.length()) / 2;
  String centeredText = "";
  for(int i = 0; i < spaces; i++) {
    centeredText += " ";
  }
  centeredText += text;
  return centeredText;
}

#ifndef CUSTOM_CHARS_H
#define CUSTOM_CHARS_H

byte one = 0b00101000;
byte two = 0b11001101;
byte three = 0b01101101;
byte four = 0b00101011;
byte five = 0b01100111;
byte six = 0b11100111;
byte seven = 0b00101101;
byte eight = 0b11101111;
byte nine = 0b00101111;
byte test = 0b00000000;

const byte FRAME_RIGHT[8]  = {
  B00111,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00111
};

const byte FRAME_LEFT[8]  = {
  B11100,
  B01100,
  B01100,
  B01100,
  B01100,
  B01100,
  B01100,
  B11100
};

const byte ARROW_LEFT[8]  = {
  B00000,
  B00100,
  B01100,
  B11111,
  B11111,
  B01100,
  B00100,
  B00000,
};

const byte ARROW_RIGHT[8]  = {
  B00000,
  B00100,
  B00110,
  B11111,
  B11111,
  B00110,
  B00100,
  B00000,
};

const byte DOLLAR_SIMBOL[8]  = {
  B00100,
  B01111,
  B10100,
  B01110,
  B00101,
  B11110,
  B00100,
  B00000
};

const byte HAMMER[8]  = {
  B00100,
  B11111,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

const byte HEART[8]  = {
  B00000,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

const byte BREAK_HEART[8]  = {
  B00000,
  B01010,
  B00110,
  B10011,
  B11011,
  B01000,
  B00100,
  B00000
};

const byte CLOVER[8]  = {
  B00100,
  B01110,
  B11111,
  B01110,
  B11111,
  B00100,
  B01110,
  B00000
};

const byte SEVEN[8]  = {
  B11111,
  B00001,
  B00010,
  B00100,
  B01000,
  B01000,
  B01000,
  B00000
};

const byte BELL[8]  = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00100,
  B01110,
  B00000
};

const byte DIAMOND[8]  = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

const byte CHERRY[8]  = {
  B00000,
  B01010,
  B00100,
  B01010,
  B11111,
  B11111,
  B01110,
  B00000
};

const byte COIN[8]  = {
  B01110,
  B10001,
  B10101,
  B10101,
  B10101,
  B10001,
  B01110,
  B00000
};

#endif
/*
  EspectrApp.h - [description]
*/
#include "Arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#ifndef Espectrapp_h
#define Espectrapp_h

//Definicao de cores
#define DARK            0x0000
#define SECONDARY       0x4249
#define SUCCESS         0xAFC2
#define WARNING         0xFD42
#define DANGER          0xF2E0
#define LIGHT           0x0415

class Espectrapp{
  public:
    Espectrapp();
    void start();
    void plotGraphLine(int);
    void fakeData();
    void setData(int,int);
    int getData(int);
    void drawBase();
    void menu();
    void clearScreen();
    void debug(String,int);
    void getTouchScreen();
    void setComando(char);
    char getComando();
    void parameterScreen();
    void salvaSD();
    void setNumMedias(int);
    void setTempoIntegracao(int);
    void startScreen();
    void blinkAlive(int);
    void sendToPhoton(String);
    void telaSalvo();
  private:
    int _data[256];
    int _counter = 0;
    int _blinkCounter = 0x0000;
    Adafruit_TFTLCD * _tft;
    TouchScreen _ts = TouchScreen(9, A2, A3, 8, 100);
    char _comando;
    char _subComando; // comandos do menu
    int _numMedias;
    int _tempoIntegracao;
    bool _started;
};

#endif

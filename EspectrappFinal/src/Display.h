/*
  EspectrApp.h - [description]
*/
#include "Arduino.h"
#include "TSL1402R.h"
#include "SdFat.h"

#ifndef Display_h
#define Display_h

class Display{
  public:
    Display(int);
    void start();
    void showMenu();
    void sendData(int, int);
    char getComando();
    void setComando(char);
    void logData(TSL1402R);
    void sendParams(int,int);
  private:
    int _baudRate = 38400;
    char _comando = 'D';
    SdFatSoftSpi<D0, D1, D3> _sd;
    // Software SPI.  Use any digital pins.
    // MISO => D0, MOSI => D1, SCK => D3, SS => D5
    const uint8_t _chipSelect = D5;
};

#endif

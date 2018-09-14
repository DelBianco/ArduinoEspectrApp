/*
  EspectrApp.h - [description]
*/
#include "Arduino.h"
#include "SdFat.h"

#ifndef TSL1402R_h
#define TSL1402R_h

class TSL1402R{
  public:
    TSL1402R(int);
    void start();
    void coletaDados();
    void demoData();
    void integra();
    void limpaSensor();
    void clockPulse();
    void siPulse();
    void clearData();
    void leConfig();

    void setTint(int);
    int getTint();
    void setNumMedias(int);
    int getNumMedias();
    int getData(int);
    int getMax();
    int getMin();
    void setLinhaDeBase();
    int getTransmissao(int);
  private:
    int _tInt = 5000;
    int _max = 0;
    int _min = 4098;
    int _numMedias = 1;
    int _clockSize = 10;
    int _data[256];
    int _base[256];
};

#endif

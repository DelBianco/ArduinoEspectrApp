/*
  EspectrApp.h - [description]
*/
#include "Arduino.h"

#ifndef PLX_DAQ_h
#define PLX_DAQ_h

class PLX_DAQ{
  public:
    PLX_DAQ(int,char);
    void start(int);
    void leituraParametros();
    void sendMSG(String);
    void resetRow();
    void sendData(int,int);
    char getComando();
    void setComando(char);
    void sendDataToArduino();
    void recuperaComando();
    float getParam(int);
  private:
    int _baudRate = 38400;
    char _comando;
    char _comandoOld;
    int _param[4] = {0,0,0,0};
};

#endif

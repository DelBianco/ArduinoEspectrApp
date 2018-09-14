/* TSL1402R.cpp */

#include "Arduino.h"
#include "Display.h"
#include "TSL1402R.h"
#include "SdFat.h"

Display::Display(int baudRate){
  _baudRate = baudRate;
}

void Display::start(){
  Serial1.begin(_baudRate,SERIAL_8E1);
  while (!Serial1) {
    // wait Serial1 port initialization
  }
  // Initialize SD
  if (!_sd.begin(_chipSelect)){
    setComando('E');
    _sd.initErrorHalt();
  }
}

char Display::getComando(){
  return _comando;
}

void Display::setComando(char comando){
  //Particle.publish("Comando",&_comando);
  _comando = comando;
  Serial1.print("#");
  Serial1.print(_comando);
  Serial1.println("#");
}

void Display::showMenu(){
  setComando('M');
}

void Display::sendData(int y, int max){
  Serial1.printlnf("%03d", map(y,0,max,0,240));
}

void Display::sendParams(int tint,int numMedias){
  setComando('S');
  Serial1.print('P');
  Serial1.print('#');
  Serial1.printlnf("%05d", tint);
  Serial1.print('#');
  Serial1.printlnf("%02d", numMedias);
  Serial1.println('#');
}

void Display::logData(TSL1402R monocromador){
  SdFile file;
  // open the file for write at end like the "Native SD library"
  int fileCount = 0;
  _sd.vwd()->rewind();
  while (file.openNext(_sd.vwd(), O_READ)){
    if (!file.isHidden()) {
      fileCount++;
    }
    file.close();
  }
  File myFile;
  String name = "log";
  name += (String)fileCount;
  name += ".txt";
  if(!myFile.open(name, O_CREAT | O_WRITE )){
      Serial1.println("MSG,Erro ao criar arquivo de log");
  }else{
      for(int i = 0; i < 256; i++){
        int data = monocromador.getData(i);
        myFile.printlnf("%d,%d",i,data);
      }
  }
  myFile.close();
}

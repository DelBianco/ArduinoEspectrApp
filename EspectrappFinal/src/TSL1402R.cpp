/* TSL1402R.cpp */

#include "Arduino.h"
#include "TSL1402R.h"
#include "SdFat.h"

TSL1402R::TSL1402R(int tInt){
  _tInt = tInt;
  _numMedias = 1;
  _clockSize = 10;
  TSL1402R::clearData();
}

void TSL1402R::start(){
  leConfig();
  TSL1402R::siPulse(); // colocando o SI em 0
  TSL1402R::clockPulse(); // colocando o Pulse em 0 (relativo ao setup)
}

int TSL1402R::getData(int i){
  return _data[i];
}


int TSL1402R::getMax(){
  return _max;
}

int TSL1402R::getMin(){
  return _min;
}
void TSL1402R::demoData(){
  for(int i=0; i < 255; i++){
    int a = analogRead(A0);
    float val = 120.0*sin(10 * i * a * 6.28 / 1048576.0) + 120;
    _data[i] = (int)val;
    if(i > 0){
      int tmpMax = max(_data[i-1],_data[i]);
      if(tmpMax > _max){
        _max = tmpMax;
      }
    }
  }
}

void TSL1402R::coletaDados(){
  // Opçao A
  TSL1402R::siPulse();
  TSL1402R::limpaSensor();
  for(int i = 0; i< _numMedias; i++){
    //*********Inicio da Medida Real
    TSL1402R::siPulse();
    TSL1402R::integra(); //********Tempo de Integração********
    TSL1402R::siPulse();
    for(int i=0; i < 256; i++){
      delayMicroseconds(20);// <-- We add a delay to stabilize the AO output from the sensor
      _data[i] += analogRead(A0);
      TSL1402R::clockPulse();
    }
  }
  // calculando a media
  _max = 0;
  _min = 4098;
  for(int i = 0; i < 256; i++){
    _data[i] = (int)_data[i]/_numMedias;
    if(i > 0){
      int tmpMin = min(_data[i-1],_data[i]);
      if(tmpMin < _min){
        _min = tmpMin;
      }
      int tmpMax = max(_data[i-1],_data[i]);
      if(tmpMax > _max){
        _max = tmpMax;
      }
    }
  }
}

void TSL1402R::integra(){
  for(long ii = 0; ii < _tInt; ii++){
    if(ii==18){
      // **********Possivel sinal de Trig*************
    }
    TSL1402R::clockPulse();
  }
}

void TSL1402R::limpaSensor(){
  for(int i=0; i < 256; i++){
    delayMicroseconds(20);
    _data[i] = 0;
    TSL1402R::clockPulse();
  }
}

void TSL1402R::clockPulse(){
  digitalWrite(D2, HIGH);
  delayMicroseconds(_clockSize);
  digitalWrite(D2, LOW);
}

void TSL1402R::siPulse(){
  digitalWrite(D4, HIGH);
  TSL1402R::clockPulse();
  digitalWrite(D4, LOW);
}

void TSL1402R::leConfig(){
  File configFile;
  if (!configFile.open("config.dat", O_READ )) {
      Serial.println("MSG,Erro ao ler arquivo de log, arquivo nao encontrado");
  }else{
    // read from the file until there's nothing else in it:
    int data;
    String paramName = "";
    String value = "";
    while ((data = configFile.read()) >= 0) {
      if((char)data == '='){
        data = configFile.read();
        while((char)data != '\n' && data >= 0){
          value += (char)data;
          data = configFile.read();
        }
        if(paramName == "tempo_integracao"){
          setTint(value.toInt());
        }
        if(paramName == "numero_medias"){
          setNumMedias(value.toInt());
        }
        if(paramName == "parametro_1"){
          //seta algo aqui
        }
        if(data >=0){
          paramName = "";
          value = "";
        }
      }else{
        paramName += (char)data;
      }
    }
    if(paramName == "tempo_integracao"){
      setTint(value.toInt());
    }
    if(paramName == "numero_medias"){
      setNumMedias(value.toInt());
    }
    if(paramName == "parametro_1"){
      //seta algo aqui
    }
  }
  configFile.close();
}

void TSL1402R::setTint(int tInt){
  _tInt = tInt;
}

int TSL1402R::getTint(){
  return _tInt;
}

void TSL1402R::setNumMedias(int numMedias){
  //Particle.publish("SetandonumMedias",(String)numMedias);
  _numMedias = numMedias;
}

int TSL1402R::getNumMedias(){
  return _numMedias;
}

void TSL1402R::clearData(){
  for(int i = 0; i < 256; i++){
    _data[i] = 0;
  }
}

void TSL1402R::setLinhaDeBase(){
  for(int i = 0; i < 256; i++){
    _base[i] = _data[i];
  }
}

int TSL1402R::getTransmissao(int i){
  return ((float)_data[i] / (float)_base[i]) * 100;
}

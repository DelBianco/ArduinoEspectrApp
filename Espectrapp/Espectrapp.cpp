/*
  Espectrapp.cpp -
*/

#include "Arduino.h"
#include "Espectrapp.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define MINPRESSURE 3
#define MAXPRESSURE 1000

Espectrapp::Espectrapp(){
  _tft = new Adafruit_TFTLCD(A3, A2, A1, A0, A4);
}

void Espectrapp::start(){
  _tft->reset();
  delay(500);
  _tft->begin(0x9341);
  _tft->setRotation(3); //Use esta linha para o controlador 9341
  clearScreen();
  startScreen(); // fazer uma tela de inicio junto com a comprovacao que a com serial com o photon ta acontecendo
}

void Espectrapp::startScreen(){
  //
}

void Espectrapp::fakeData(){
  for(int i =0; i < 256; i++){
    float val = 120*sin(i * 6.28 / 256.0);
    _data[i] = 120+(int)val;
  }
}

void Espectrapp::plotGraphLine(int COR){
  _tft->drawFastVLine(11, 11, 220, DARK);
  for(int i = 1 ; i<255; i++){
    _tft->drawFastVLine(11 + i, 11, 220, DARK);
    if(_data[i] >= 0){
      _tft->drawLine(i+10 , _tft->height() - _data[i-1]  , i+11 , _tft->height() -_data[i]   , SUCCESS);
    }
  }
}

void Espectrapp::setData(int data,int i){
  if(data > 240){
    data = 240;
  }
  if(data < 0){
    data = 0;
  }
  if( i >= 0 && i<256){
    _data[i] = map(data , 0 , 240, 11 , _tft->height() - 11);
  }
}

int Espectrapp::getData(int i){
  return _data[i];
}

void Espectrapp::clearScreen(){
  _tft->fillScreen(DARK);
}
void Espectrapp::drawBase(){
  _tft->fillScreen(DARK);
  _tft->drawRect(10, 10,257, _tft->height() -18, LIGHT);

  _tft->setTextColor(LIGHT);
  _tft->setTextSize(1);
  _tft->setCursor(10, _tft->height() - 9);
  _tft->println("0");
  _tft->setCursor(265, _tft->height() - 9);
  _tft->println("256");
}

void Espectrapp::debug(String msg,int i){
  int TS = 1;
  _tft->setTextColor(DANGER);
  _tft->setTextSize(TS);
  _tft->setCursor(12, 12*TS*i);
  _tft->println(msg);
}

void Espectrapp::getTouchScreen(){
  TSPoint p = _ts.getPoint();
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    p.x = _tft->width() - (map(p.x, 150, 920, 0, _tft->width()));
    p.y = _tft->height() - (map(p.y, 120, 940, 0, _tft->height()));
    //Serial.print("py: ");Serial.print(p.y);Serial.print(" px: ");Serial.println(p.x);Serial.print("Comando: ");Serial.println(_comando);Serial.println("---------------");
    if(_comando == 'M'){
      if(_subComando == 'P'){ // mapeamento dos cliques na tela de parametros
          _comando = 'G';
          _subComando = 'A';
      }else{
        if(p.x > 110 && p.x < 220){
          _subComando = 'P';
          parameterScreen();
        }else{
          if(p.x > 220){
            salvaSD();
            _comando = 'O';
          }
          _comando = 'G';
        }
      }
    }else{
      if(_comando == 'G'){
        _comando = 'M';
      }
      if(_comando == 'O'){
        telaSalvo();
      }
    }
  }
}

void Espectrapp::telaSalvo(){
  _tft->fillScreen(DARK);
  _tft->setTextColor(DANGER);
  _tft->setTextSize(2);
  _tft->setCursor(10, _tft->height()/2);
  _tft->println("Dados Salvos");
}

void Espectrapp::menu(){
  _tft->fillScreen(DARK);
  _tft->setTextColor(WARNING);
  _tft->setTextSize(2);
  _tft->setCursor(10, 10);
  _tft->println("MENU");
  _tft->drawLine(0 , 35  , _tft->width() , 35   , DANGER);

  _tft->setTextSize(15);
  _tft->setCursor(10, 60);
  _tft->println("<");

  _tft->setTextSize(2);
  _tft->setCursor(10,180);
  _tft->println("Voltar");
  _tft->setCursor(10,200);
  _tft->println("<-----");

  _tft->setTextSize(15);
  _tft->setCursor(115, 60);
  _tft->println("P");

  _tft->setTextSize(2);
  _tft->setCursor(115,180);
  _tft->println("Ajustar");
  _tft->setTextSize(1);
  _tft->setCursor(115,200);
  _tft->println("Parametros");

  _tft->setTextSize(15);
  _tft->setCursor(220, 60);
  _tft->println("S");

  _tft->setTextSize(2);
  _tft->setCursor(220,180);
  _tft->println("Salvar");
  _tft->setCursor(220,200);
  _tft->println("Dados");

}

// Desenha a tela de parametros
void Espectrapp::parameterScreen(){
  _tft->fillScreen(DARK);
  _tft->setTextColor(DANGER);
  _tft->setTextSize(2);
  _tft->setCursor(10, 10);
  _tft->println("AJUSTANDO PARAMETROS");
  _tft->drawLine(0 , 35  , _tft->width() , 35   , DANGER); // TODO: trocar por fastHline
  _tft->setCursor(10, 45);
  _tft->setTextSize(2);
  _tft->println("Tempo de Integracao ");
  _tft->setTextSize(1);
  _tft->setCursor(10,65);
  _tft->println("(Tempo de exposicao do sensor a LUZ)");

  _tft->setTextColor(LIGHT);
  _tft->fillRect(10, 150, 50, 50, DARK);
  _tft->drawRect(10, 150, 50, 50, SUCCESS);
  _tft->setTextSize(4);
  _tft->setCursor(20,150);
  _tft->println("-");

  _tft->fillRect(70, 150, 50, 50, DARK);
  _tft->drawRect(70, 150, 50, 50, SUCCESS);
  _tft->setTextSize(4);
  _tft->setCursor(90,180);
  _tft->println("+");
}

// Envia o comando para salvar no SD e informa o usuário se teve sucesso ou nao
void Espectrapp::salvaSD(){
  sendToPhoton("SALVA_SD");
}

void Espectrapp::setNumMedias(int numMedias){
  _numMedias = numMedias;
}

void Espectrapp::setTempoIntegracao(int tInt){
  _tempoIntegracao = tInt;
}

void Espectrapp::setComando(char comando){
  _comando = comando;
}

char Espectrapp::getComando(){
  return _comando;
}

void Espectrapp::sendToPhoton(String str){
  Serial.print("#");
  Serial.print(str);
  Serial.println("#");
}

void Espectrapp::blinkAlive(int offset){
  if(_blinkCounter == 0xFFFF) _blinkCounter = 0x0000;
  _blinkCounter+= 0x1111;
  _tft->fillCircle(_tft->width() -5, _tft->height() -5 + offset, 3, _blinkCounter);
}

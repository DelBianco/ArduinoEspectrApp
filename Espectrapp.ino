#include <Espectrapp.h>

Espectrapp app;
bool dataChanged = false;
bool menu = false;

void setup(){
  Serial.begin(115200,SERIAL_8E1);
  while(!Serial){
    // Wait serial port initialization
  }
  app.start();
  app.drawBase();
  app.setComando('M');
}

void loop(){
  app.getTouchScreen();
  if(app.getComando() == 'G'){
    if(menu){
      app.drawBase();
    }
    if(dataChanged){
      app.plotGraphLine(0x3B77);
      dataChanged = false;
    }
    menu = false;
  }
  if(app.getComando() == 'M'){
    if(!menu){
      app.menu();
    }
    menu=true;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent(){
  while(Serial.available()){
    app.blinkAlive(0);
    char inChar;
    if(inChar != '#'){
      inChar = (char)Serial.read();
    }else{
      inChar = (char)Serial.read();
      if(inChar == 'D' || inChar == 'M' || inChar == 'G' || inChar == 'S'){
        //Serial.println(inChar);
        Serial.read();Serial.read(); // Trailling
      }
      if(inChar == 'D'){ // dados vindo espera
        while(!Serial.available()){} 
        Serial.read(); // # Trailling
        for(int i = 0; i < 256; i++){
          while (Serial.available() < 5) {} // Espera ter certeza de que tem um dado do tipo 123\n no buffer
          int data = Serial.parseInt();
          app.setData(data,i);
          Serial.read(); // \n
        }
        dataChanged = true;
      }
      if(inChar == 'S'){ // Starting
        while(Serial.available() < 11){}
        inChar = (char)Serial.read(); // P
        inChar = (char)Serial.read(); // P
        Serial.println(inChar);
        if(inChar == 'P'){
          Serial.read(); // #
          app.setTempoIntegracao(Serial.parseInt());
          Serial.read(); // #
          app.setNumMedias(Serial.parseInt());
          Serial.read(); // #
          Serial.read(); // \n
        }
      }
    }
  }
}

// SYSTEM_MODE(MANUAL);
#include "TSL1402R.h"
#include "Display.h"
#include "PLX_DAQ.h"

int buffer[8][256];
int maxBuffer = 8;
int bufferCount = 0;
int menu = 1;
bool click = false;
void btnPress(void);

PLX_DAQ excelComm(38400,'C');
Display display(115200);
TSL1402R monocromador(5000);

void setup(){
  // Initialize two pins as digital output:
  pinMode(D2, OUTPUT);  // D2 envia CLK pulse to pin 2 of the TSL1402R
  pinMode(D4, OUTPUT);   // D4 Pino de SI 4 (analog output 1) of the TSL1402R
  pinMode(D6, INPUT_PULLUP); // pino de interrupcao do botao

  display.start();
  monocromador.start(); // inicializa o monocromador, elimina qualquer dado que esteja flutuando lá
  excelComm.start(2000);
  display.sendParams(monocromador.getTint(),monocromador.getNumMedias());
  attachInterrupt(D6, btnPress, FALLING);
}

void loop(){
  noInterrupts();
  monocromador.coletaDados();
  // monocromador.demoData();
  interrupts();

  if(click){
    // trigger do click no botao
    // display.logData(monocromador);
    excelComm.setComando('A');
    click = false;
  }
  processaComandos();
  excelComm.leituraParametros();
}

void processaComandos(){
  if(excelComm.getComando() == 'A'){
    excelComm.sendMSG("Exibindo padrao de transmisao");
    display.setComando('D');
    for(int i=0;i<256;i++){
      excelComm.sendData(i,monocromador.getTransmissao(i));
      display.sendData(monocromador.getTransmissao(i),monocromador.getMax());
    }
    excelComm.resetRow();
  }

  if(excelComm.getComando() == 'B'){
    excelComm.sendMSG("Coletando linha de base");
    monocromador.setLinhaDeBase();
    excelComm.setComando('Z');
  }

  if(excelComm.getComando() == 'C'){
    excelComm.sendMSG("Tempo Real");
    display.setComando('D');
    for(int i=0;i<256;i++){
      excelComm.sendData(i,monocromador.getData(i));
      display.sendData(monocromador.getData(i),monocromador.getMax());
    }
    excelComm.resetRow();
  }

  // Seta os parametros
  if(excelComm.getComando() == 'E'){
    excelComm.sendMSG("Setando Parametros");
    excelComm.leituraParametros();
    monocromador.setTint(excelComm.getParam(0));
    monocromador.setNumMedias(excelComm.getParam(1));
    display.sendParams(monocromador.getTint(),monocromador.getNumMedias());
    excelComm.recuperaComando();
  }

  if(excelComm.getComando() == 'F'){
    excelComm.recuperaComando();
    display.logData(monocromador);
  }
}

void btnPress(){
  while(digitalRead(D6) == 0){}
  click = true;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1(){
  while(Serial1.available()){
    char inChar;
    inChar = (char)Serial1.read();
    if(inChar == '#'){
      inChar = (char)Serial1.read();
      if(inChar != '\n'){
        while (Serial1.available() < 9) {}
        String comando = "";
        while(inChar != '#'){
          comando += inChar;
          inChar = (char)Serial1.read();
        }
        if(comando == "SALVA_SD"){
          excelComm.setComando('F');
        }
      }
    }
  }
}

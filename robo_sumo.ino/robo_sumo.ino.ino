/*******************************************************************************************************
------------------------------------ Autor : Gustavo R Stroschon -----------------------------------------
------------------------------------ Date: 21/02/2020 ----------------------------------------------------
------------------------------------ Função do programa: código utilizado para a demonstração do robô sumo
------------------------------------ --------------------------------------------- -----------------------
------------------------------------ Revisado por: Victor Eduardo Ferreira (RC0D3) -----------------------
------------------------------------ Data: 23/08/2024 ----------------------------------------------------
------------------------------------ Atualização: Adicionado sensor ultrassônico -------------------------
------------------------------------ --------------------------------------------- -----------------------
------------------------------------ Revisado por: Victor Eduardo Ferreira (RC0D3) -----------------------
------------------------------------ Data: 10/09/2025 ----------------------------------------------------
------------------------------------ Atualização: Troca Push Button NF para NA ---------------------------
*******************************************************************************************************/
 
 
// ===== ATENÇÃO =====
// Robôs comprados antes da data 10/09/2025 é necessário mudar a linha 45
// Substituindo "while (digitalRead(2) == 1) {"
// para "while (digitalRead(2) == 0) {"
 
//Incluir biblioteca V 3.0.0
#include <Ultrasonic.h>
 
Ultrasonic ultrasonic(8, 7);  // Trigger e Echo
// int encoderPin = 3;
// volatile unsigned long totalPulses = 0;
// //20 buracos no disco para 360 graus, então pela divisão dá 18 graus por buraco
// volatile int anglePerPulse = 18;
// //angulo total calculado pelos pulsos totais
// volatile long totalAngle = 0;

// //variaveis para medir tempo
// volatile unsigned long lastTime =0;
// volatile unsigned long currentTime=0;
// volatile double deltaTime = 0;

// //velocidade calculada a partir das variaveis acima em RPM
// volatile double rpm=0;
// //tempo total até que a soma dos pulsos chegue à pulsosMedicao
// volatile unsigned long timeAvgVelocidade = 0;
// //somaPulsos vai de 0 a avgSample e depois vai a 0
// volatile int somaPulsos=0;
// //qts pulsos vai levar pra calcular a média de velocidade
// volatile int pulsosMedicao = 50;
// // Pulsos por revolução (360 / anglePerPulse)
// const int pulsesPerRev = 20;

void setup() {
  // pinMode(encoderPin, INPUT);   // Sensor de velocidade encoder
  // attachInterrupt(digitalPinToInterrupt(encoderPin), interruptFunction, RISING); // 

  pinMode(11, INPUT);  // Sensor de linha
  pinMode(12, INPUT);  // Sensor de linha
  pinMode(13, INPUT);  // Sensor de linha
 
  pinMode(2, INPUT_PULLUP);  // Botão de Ligar
 
  pinMode(5, OUTPUT);   // Ponte H B-IA
  pinMode(6, OUTPUT);   // Ponte H B-IB
  pinMode(9, OUTPUT);   // Ponte H A-IA
  pinMode(10, OUTPUT);  // Ponte H A-IB
 
  pinMode(8, OUTPUT);  // Trigger Sensor Ultrassônico
  pinMode(7, INPUT);    // Echo Sensor Ultrassônico
 
 
 
  Serial.begin(9600);

  lastTime = millis();  
 
  while (digitalRead(2) == 1) { // Alterar caso necessário conforme aviso acima
    delay(1);
    stop();
    Serial.println("Aguardando ligar...");
  }
}


void loop() {
 
  if (!digitalRead(12) || !digitalRead(13)) {  // Verifica sensores de linha
    //Serial.println("Linha detectada");
    stop();
    delay(1);
    re();
    delay(300);
    stop();
    delay(1);
    girar();
    delay(600);
    stop();
  }
 
  int centimetros = ultrasonic.read(CM);
 
  if (centimetros < 30) {
    //Serial.println("Oponente detectado");
    stop();
    delay(1);
    atacar();
    delay(300);
    stop();
  }
 
  frente();
  delay(1);
}

// void interruptFunction(){
//   currentTime=millis();
//   deltaTime=currentTime-lastTime;
//   //increment the total number of pulses
//   totalPulses ++;
//   totalAngle= totalPulses * anglePerPulse;

//   somaPulsos++;
//   timeAvgVelocidade += deltaTime;

//   if(somaPulsos >= pulsosMedicao){
//     rpm = ((double)somaPulsos * 60000.0) / ((double)timeAvgVelocidade * pulsesPerRev);
//     Serial.print("Velocidade: ");
//     Serial.print(rpm);
//     Serial.println(" RPM");
//     somaPulsos=0;
//     timeAvgVelocidade=0;
//   }

//   lastTime=millis();

// }
 
void re() {
  //Serial.println("Re");
  analogWrite(5, 0);
  analogWrite(6, 255);
  analogWrite(9, 0);
  analogWrite(10, 255);
}
void frente() {
  //Serial.println("Frente");
  analogWrite(5, 255);
  analogWrite(6, 0);
  analogWrite(9, 255);
  analogWrite(10, 0);
}
void girar() {
  //Serial.println("Girando");
  analogWrite(5, 0);
  analogWrite(6, 255);
  analogWrite(9, 255);
  analogWrite(10, 0);
}
void atacar() {
  //Serial.println("Atacar");
  analogWrite(5, 255);
  analogWrite(6, 0);
  analogWrite(9, 255);
  analogWrite(10, 0);
}
void stop() {
  //Serial.println("Parado");
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, 0);
  analogWrite(10, 0);
}
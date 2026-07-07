#include <Ultrasonic.h>

// Pinos dos Sensores de Linha
int s_linha_tras = A1;
int s_linha_esq = A2;
int s_linha_dir = A0;

const int limiar_dir = 70; //46
const int limiar_esq = 40; //26
const int limiar_tras = 50; //34

Ultrasonic ultrasonic(8, 7);  // Trigger(8) e Echo(7)

void setup() {
  pinMode(2, INPUT_PULLUP);  // Botão de Ligar

  // Configuração dos pinos da Ponte H
  pinMode(6, OUTPUT);   // Ponte H B-IA
  pinMode(5, OUTPUT);   // Ponte H B-IB
  pinMode(10, OUTPUT);  // Ponte H A-IA
  pinMode(9, OUTPUT);   // Ponte H A-IB
  
  Serial.begin(9600);
  
  // Aguarda o botão ser pressionado para iniciar
  while (digitalRead(2) == 1) { 
    delay(1);
    stop();
    Serial.println("Aguardando ligar...");
  }

  Serial.println("Botão pressionado! Iniciando em 3 segundos...");
  stop();       // Garante que os motores fiquem totalmente travados durante a contagem
  delay(3000);
  Serial.println("Iniciando o loop!");

}

void loop() {

  // 1. Faz a leitura numérica de todos os sensores
  int leitura_tras = analogRead(s_linha_tras);
  int leitura_esq = analogRead(s_linha_esq);
  int leitura_dir = analogRead(s_linha_dir);

  // 2. Transforma os números em "Verdadeiro" ou "Falso" (Facilita a leitura do código)
  bool tocou_tras = (leitura_tras > limiar_tras);
  bool tocou_esq = (leitura_esq > limiar_esq);
  bool tocou_dir = (leitura_dir > limiar_dir);

  // === ESTRATÉGIA DE DEFESA (SENSORES DE LINHA) ===
  // O código agora avalia do cenário mais crítico para o mais simples
  
  // Combinação: Bateu os DOIS sensores frontais na linha ao mesmo tempo
  if (tocou_esq && tocou_dir) {
    stop();
    re();
    delay(400);         // Recua um pouco mais de tempo
    stop();
    girar_direita();    // Como bateu de frente, faz um giro maior (ex: 180 graus) para voltar ao centro
    delay(900);         
    stop();
  }
  
  // Individual: Tocou APENAS o da Esquerda
  else if (tocou_esq) {
    stop();
    re();
    delay(400);         
    stop();
    girar_direita();    // Foge para o lado oposto
    delay(1000);
    stop();
  }
  
  // Individual: Tocou APENAS o da Direita
  else if (tocou_dir) {
    stop();
    re();
    delay(400);         
    stop();
    girar_esquerda();   // Foge para o lado oposto
    delay(700);
    stop();
  }
  
  // Cuidado Máximo: A traseira tocou na linha!
  else if (tocou_tras) {
    stop();
    frente_busca();           // Acelera para o centro imediatamente
    delay(400);
    stop();
  }
  // === ESTRATÉGIA DE ATAQUE (Nenhuma linha detectada) ===
  int centimetros = ultrasonic.read(CM);
  //Serial.print(centimetros);
  //Serial.println();
  
  // Ignora o valor 357 (erro de leitura) e procura objetos a menos de 30cm
  if (centimetros < 30) {
    atacar();
    delay(200);
  }else{
  frente_busca();
  }
  
  delay(1);
}

// ===== FUNÇÕES DE MOVIMENTAÇÃO =====
void re() {
  analogWrite(5, 0);
  analogWrite(6, 255);
  analogWrite(9, 0);
  analogWrite(10, 255);
}

void frente_busca() {
  // Velocidade reduzida para dar tempo de ler a linha e frear
  analogWrite(5, 160); 
  analogWrite(6, 0);
  analogWrite(9, 160);
  analogWrite(10, 0);
}

void girar_esquerda() {
  analogWrite(5, 0);
  analogWrite(6, 255);
  analogWrite(9, 255);
  analogWrite(10, 0);
}

void girar_direita() {
  analogWrite(5, 255);
  analogWrite(6, 0);
  analogWrite(9, 0);
  analogWrite(10, 255);
}

void atacar() {
  analogWrite(5, 255);
  analogWrite(6, 0);
  analogWrite(9, 255);
  analogWrite(10, 0);
}

void stop() {
  // Enviar 255 para todos os pinos da Ponte H trava os motores na hora
  analogWrite(5, 255);
  analogWrite(6, 255);
  analogWrite(9, 255);
  analogWrite(10, 255);
}
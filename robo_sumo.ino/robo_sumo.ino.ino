#include <Ultrasonic.h>

// Pinos dos Sensores de Linha
int s_linha_tras = A1;
int s_linha_esq = A0;
int s_linha_dir = A2;

const int LIMIAR_LINHA = 100; 

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
}

void loop() {
  // 1. Faz a leitura numérica de todos os sensores
  int leitura_tras = analogRead(s_linha_tras);
  int leitura_esq = analogRead(s_linha_esq);
  int leitura_dir = analogRead(s_linha_dir);


  // 2. Transforma os números em "Verdadeiro" ou "Falso" (Facilita a leitura do código)
  bool tocou_tras = (leitura_tras > LIMIAR_LINHA);
  bool tocou_esq = (leitura_esq > LIMIAR_LINHA);
  bool tocou_dir = (leitura_dir > LIMIAR_LINHA);

  // === ESTRATÉGIA DE DEFESA (SENSORES DE LINHA) ===
  // O código agora avalia do cenário mais crítico para o mais simples

  // Cuidado Máximo: A traseira tocou na linha!
  if (tocou_tras) {
    stop();
    frente_busca();           // Acelera para o centro imediatamente
    delay(600);
    stop();
  }
  
  // Combinação: Bateu os DOIS sensores frontais na linha ao mesmo tempo
  else if (tocou_esq && tocou_dir) {
    stop();
    re();
    delay(600);         // Recua um pouco mais de tempo
    stop();
    girar_direita();    // Como bateu de frente, faz um giro maior (ex: 180 graus) para voltar ao centro
    delay(600);         
    stop();
  }
  
  // Individual: Tocou APENAS o da Esquerda
  else if (tocou_esq) {
    stop();
    re();
    delay(200);         
    stop();
    girar_direita();    // Foge para o lado oposto
    delay(300);
    stop();
  }
  
  // Individual: Tocou APENAS o da Direita
  else if (tocou_dir) {
    stop();
    re();
    delay(200);         
    stop();
    girar_esquerda();   // Foge para o lado oposto
    delay(300);
    stop();
  }
  
  // === ESTRATÉGIA DE ATAQUE (Nenhuma linha detectada) ===
  else {
    int centimetros = ultrasonic.read(CM);
    
    // Ignora o valor 357 (erro de leitura) e procura objetos a menos de 30cm
    if (centimetros < 30 && centimetros > 0) {
      atacar(); 
    } else {
      frente_busca(); // Modo de busca
    }
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
  analogWrite(5, 150); 
  analogWrite(6, 0);
  analogWrite(9, 150);
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

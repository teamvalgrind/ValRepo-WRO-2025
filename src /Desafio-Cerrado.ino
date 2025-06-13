#include <Wire.h>
#include <Ultrasonic.h>
#include <ESP32Servo.h>
#include <Pixy2.h>

// Pines ESP32 para sensores ultrasónicos
#define USTFRONT 13
#define USEFRONT 12
#define USTLEFT 14
#define USELEFT 27
#define USTRIGHT 26
#define USERIGHT 25

// Pines para el driver de motor y servo
#define IN2 17
#define IN1 16
#define PIN_SERVO 19
#define PIN_BOTON 4

// Valores para servo y Pixy2
#define CENTERVALUE 99
#define MAXLEFT 150
#define MAXRIGHT 30
#define SPEED2 1

#define X_REF_VERDE 100
#define X_REF_ROJO 200
#define X_REF_MAGENTA 60

Ultrasonic USFront(USTFRONT, USEFRONT);
Ultrasonic USLeft(USTLEFT, USELEFT);
Ultrasonic USRight(USTRIGHT, USERIGHT);
Servo direccion;
Pixy2 pixy;

const int DISTANCIA_OBSTACULO_FRONTAL = 20;
const int DISTANCIA_OBSTACULO_LATERAL = 190;
const unsigned long DURACION_GIRO_MS = 750;
const unsigned long DURACION_GIRO_BLOQUE_MS = 350;
const unsigned long TIEMPO_ESPERA_GIRO = 2500;

bool programaIniciado = false;
bool finalizado = false;
bool following = false;
int currentGrade = CENTERVALUE;

unsigned long tiempoUltimoGiro = 0;
int contadorGiros = 0;

#define DEBUG 1

void setup() {
  direccion.attach(PIN_SERVO);
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  direccion.write(CENTERVALUE);
  delay(3000);

  pixy.init();
  pixy.setLamp(1, 0);

  Serial.println("Esperando pulsar botón para iniciar...");
}

void loop() {
  if (!programaIniciado) {
    if (digitalRead(PIN_BOTON) == LOW) {
      programaIniciado = true;
      Serial.println("Botón presionado, iniciando programa...");
      delay(500);  // debounce
    }
  } else if (!finalizado) {
    controlarRobot();
  }
}

void controlarRobot() {
  unsigned long ahora = millis();
  int frontal = USFront.read();
  int izquierda = USLeft.read();
  int derecha = USRight.read();

  if (frontal == 357) frontal = -1;
  if (izquierda == 357) izquierda = -1;
  if (derecha == 357) derecha = -1;

  pixy.ccc.getBlocks();
  printAllValues();

  if (pixy.ccc.numBlocks > 0) {
    for (uint16_t i = 0; i < pixy.ccc.numBlocks; i++) {
      uint8_t sig = pixy.ccc.blocks[i].m_signature;
      int x = pixy.ccc.blocks[i].m_x;
      int y = pixy.ccc.blocks[i].m_y;
      if (sig == 4) { // Verde
        goToPosition(XVALUEGREEN, 200, x, y);
        return;
      } else if (sig == 3) { // Rojo
        goToPosition(XVALUERED, 200, x, y);
        return;
      } else if (sig == 5) { // Magenta
        goToPosition(XVALUEMAG1, 200, x, y);
        return;
      }
    }
  }

  // Lógica de obstáculos y vueltas
  if (contadorGiros >= 12) {
    if (!finalizado) {
      Adelante();
      delay(1000);
      Parar();
      finalizado = true;
    }
    return;
  }

  if (frontal > DISTANCIA_OBSTACULO_FRONTAL || frontal == -1) {
    Adelante();
    if (ahora - tiempoUltimoGiro >= TIEMPO_ESPERA_GIRO) {
      if (izquierda > DISTANCIA_OBSTACULO_LATERAL || izquierda == -1) {
        Parar();
        Izquierda();
        contadorGiros++;
        tiempoUltimoGiro = millis();
        Adelante();
      } else if (derecha > DISTANCIA_OBSTACULO_LATERAL || derecha == -1) {
        Parar();
        Derecha();
        contadorGiros++;
        tiempoUltimoGiro = millis();
        Adelante();
      }
    }
  } else if (frontal != -1 && frontal <= DISTANCIA_OBSTACULO_FRONTAL) {
    Parar();
  }
}

// --- Lógica Pixy2 y movimiento ---

void goToPosition(int targetX, int targetY, int currentX, int currentY) {
  int pos = CENTERVALUE;  // Inicialmente recto
  if (DEBUG) Serial2.println("Target: " + String(targetX) + " , " + String(targetY));
  if (DEBUG) Serial2.println("Current: " + String(currentX) + " , " + String(currentY));
  following = true;
  if (currentY < targetY) {
    if (targetX == XVALUEGREEN) {  // Para el color verde
      if (DEBUG) Serial2.println("Para el color verde");
      if (currentX < targetX) {
        pos = map(currentX, 0, targetX, MAXLEFT, CENTERVALUE);
        if (DEBUG) Serial2.println("Para el color verde corrige");
      } else {
        pos = CENTERVALUE - 6;
        if (DEBUG) Serial2.println("Para el color verde sigue recto");
      }
    } else if (targetX == XVALUERED) {  // Para el color rojo
      if (DEBUG) Serial2.println("Para el color rojo");
      if (currentX > targetX) {
        pos = map(currentX, targetX, 320, CENTERVALUE, MAXRIGHT);
        if (DEBUG) Serial2.println("Para el color rojo corrige");
      } else {
        pos = CENTERVALUE + 10;
        if (DEBUG) Serial2.println("Para el color rojo sigue recto");
      }
    } else if (targetX == XVALUEMAG1) { // Magenta 1
      if (currentX > targetX) {
        pos = map(currentX, 0, targetX, MAXLEFT, CENTERVALUE);
        if (DEBUG) Serial2.println("Para mangenta 1 corrige");
      } else {
        pos = CENTERVALUE + 6;
        if (DEBUG) Serial2.println("Para mangenta 1 recto");
      }
    } else if (targetX == XVALUEMAG2) { // Magenta 2
      if (currentX > targetX) {
        pos = map(currentX, targetX, 320, CENTERVALUE, MAXRIGHT);
        if (DEBUG) Serial2.println("Para mangenta 2 corrige");
      } else {
        pos = CENTERVALUE + 6;
        if (DEBUG) Serial2.println("Para mangenta 2 sigue recto");
      }
    }
    currentGrade = pos;
    direccion.write(pos);  // Ajustar el servo
    if (DEBUG) Serial2.println("Posicion Servo: " + String(pos));
    Adelante();  // Avanzar
  }
}

void printValues(byte mIndex) {
  switch (pixy.ccc.blocks[mIndex].m_signature) {
    case 0:
      if (DEBUG) Serial2.print(F("Ninguno"));
      if (DEBUG) Serial2.print(mIndex);
      break;
    case 1:
      if (DEBUG) Serial2.print(F("Azul "));
      break;
    case 2:
      if (DEBUG) Serial2.print(F("Naranja "));
      break;
    case 3:
      if (DEBUG) Serial2.print(F("Rojo "));
      break;
    case 4:
      if (DEBUG) Serial2.print(F("Verde "));
      break;
    case 5:
      if (DEBUG) Serial2.print(F("Magenta "));
      break;
    default:
      if (DEBUG) Serial2.print(F("Ninguno. Signature "));
      if (DEBUG) Serial2.print(mIndex);
  }
  if (DEBUG) Serial2.print("  X: " + String(pixy.ccc.blocks[mIndex].m_x));
  if (DEBUG) Serial2.println("  Y: " + String(pixy.ccc.blocks[mIndex].m_y));
}

void printAllValues() {
  if (DEBUG) Serial2.println();
  for (byte b = 0; b < 40; b++)
    if (DEBUG) Serial2.print("-");
  if (DEBUG) Serial2.println();

  for (byte i = 0; i < pixy.ccc.numBlocks; i++)
    printValues(i);

  for (byte b = 0; b < 40; b++)
    if (DEBUG) Serial2.print("-");
  if (DEBUG) Serial2.println();
}

// --- Movimiento con driver de motor ---

void Adelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  if (DEBUG) Serial2.println("Motor en marcha hacia adelante");
}

void Parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  if (DEBUG) Serial2.println("Motor detenido");
}

void Izquierda() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  direccion.write(MAXLEFT);
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_MS) delay(10);
  direccion.write(CENTERVALUE);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  if (DEBUG) Serial2.println("Giro izquierda completado");
}

void Derecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  direccion.write(MAXRIGHT);
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_MS) delay(10);
  direccion.write(CENTERVALUE);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  if (DEBUG) Serial2.println("Giro derecha completado");
}
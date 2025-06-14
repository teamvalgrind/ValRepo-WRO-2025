#include <Wire.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <Pixy2SPI_SS.h>

#define IN1 16
#define IN2 17
#define PIN_SERVO 2
#define PIN_BOTON 4

Pixy2SPI_SS pixy;
Servo myservo;

bool programaIniciado = false;

const int UMBRAL_Y = 15;          // Umbral para activar detección según Y
const int UMBRAL_IZQUIERDA = 115; // Umbral para determinar izquierda/derecha

bool girando = false;
bool giroIzqActivo = false;
unsigned long tiempoInicioGiro = 0;
const unsigned long TIEMPO_GIRO_SUAVE = 600;    // Duración giro suave izquierda (ms)
const unsigned long TIEMPO_CORRECCION = 200;    // Duración corrección derecha (ms)

void setup() {
  Serial.begin(115200);
  myservo.attach(PIN_SERVO);

  pinMode(PIN_BOTON, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  myservo.write(97);  // Servo centrado

  SPI.begin(18, 19, 23, 5);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  if (pixy.init() == 0) {
    Serial.println("Pixy2 inicializada correctamente.");
  } else {
    Serial.println("Error al inicializar Pixy2.");
  }
  pixy.changeProg("color_connected_components");
  pixy.setLamp(1, 0); // Enciende luces Pixy2

  Serial.println("Esperando pulsar botón para iniciar...");
}

void loop() {
  if (!programaIniciado) {
    if (digitalRead(PIN_BOTON) == LOW) {
      programaIniciado = true;
      Serial.println("Programa iniciado.");
      delay(500);
    }
    return;
  }

  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    int y = pixy.ccc.blocks[0].m_y;
    int x = pixy.ccc.blocks[0].m_x;
    uint8_t sig = pixy.ccc.blocks[0].m_signature;

    Serial.print("Firma detectada: ");
    Serial.println(sig);
    Serial.print("Posición X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.println(y);

    if (sig == 1 && y > UMBRAL_Y) {  // Solo bloque verde y Y > umbral
      if (!girando) {
        girando = true;
        tiempoInicioGiro = millis();

        if (x > UMBRAL_IZQUIERDA) {
          Serial.println("Bloque verde a la derecha: Avanzar recto");
          Adelante();
          giroIzqActivo = false; // No gira
        } else {
          Serial.println("Bloque verde a la izquierda: Giro suave izquierda");
          giroIzquierdaSuave();
          giroIzqActivo = true;
        }
      } else {
        Adelante();
      }
    } else {
      // Cuando bloque no detectado o Y bajo umbral, si giró a la izquierda, corrige a la derecha
      if (girando && millis() - tiempoInicioGiro >= TIEMPO_GIRO_SUAVE) {
        if (giroIzqActivo) {
          Serial.println("Corrección giro derecha para enderezar");
          correccionDerecha();
        }
        girando = false;
      } else {
        Adelante();
      }
    }
  } else {
    // Si no detecta bloques y estaba girando, corrige
    if (girando && millis() - tiempoInicioGiro >= TIEMPO_GIRO_SUAVE) {
      if (giroIzqActivo) {
        Serial.println("Corrección giro derecha para enderezar");
        correccionDerecha();
      }
      girando = false;
    } else {
      Adelante();
    }
  }
}

void Adelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  myservo.write(97);
  Serial.println("Avanzando recto");
}

void giroIzquierdaSuave() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  myservo.write(140);  // Giro suave a la izquierda
  Serial.println("Girando suavemente a la izquierda");
}

void correccionDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  myservo.write(30);   // Giro leve a la derecha para corrección
  delay(TIEMPO_CORRECCION);
  myservo.write(97);   // Centrar servo
  Serial.println("Corrección giro derecha completada");
}
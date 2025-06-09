#include <Wire.h>
#include <Ultrasonic.h>
#include <ESP32Servo.h>
#include <Pixy2.h>

#define USTFRONT 13
#define USEFRONT 12
#define USTLEFT 14
#define USELEFT 27
#define USTRIGHT 26
#define USERIGHT 25

#define PIN_ESC 18
#define PIN_SERVO 19
#define PIN_BOTON 4

Ultrasonic USFront(USTFRONT, USEFRONT);
Ultrasonic USLeft(USTLEFT, USELEFT);
Ultrasonic USRight(USTRIGHT, USERIGHT);

Servo esc;
Servo myservo;
Pixy2 pixy;

const int DISTANCIA_OBSTACULO_FRONTAL = 20;
const int DISTANCIA_OBSTACULO_LATERAL = 190;
const unsigned long DURACION_GIRO_MS = 750;
const unsigned long DURACION_GIRO_BLOQUE_MS = 350; // Giro leve ~45°
const unsigned long TIEMPO_ESPERA_GIRO = 2500;

const uint8_t ROJO_SIGNATURE = 1;
const uint8_t VERDE_SIGNATURE = 2;

bool motorEnMarcha = false;
bool girando = false;
bool programaIniciado = false;
bool finalizado = false;

unsigned long tiempoUltimoGiro = 0;
int contadorGiros = 0;

void setup() {
  esc.attach(PIN_ESC, 1000, 2000);
  myservo.attach(PIN_SERVO);
  Serial.begin(115200);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  esc.write(90);
  myservo.write(100);
  delay(3000);
  Serial.println("Esperando pulsar botón para iniciar...");
  pixy.init();
  pixy.setLamp(1, 0);
}

void loop() {
  if (!programaIniciado) {
    if (digitalRead(PIN_BOTON) == LOW) {
      programaIniciado = true;
      Serial.println("Botón presionado, iniciando programa...");
      delay(500);
    }
  } else if (!finalizado) {
    docegiros_pixy();
  }
}

void docegiros_pixy() {
  unsigned long ahora = millis();
  int frontal = USFront.read();
  int izquierda = USLeft.read();
  int derecha = USRight.read();

  if (frontal == 357) frontal = -1;
  if (izquierda == 357) izquierda = -1;
  if (derecha == 357) derecha = -1;

  pixy.ccc.getBlocks();
  bool bloqueRojo = false;
  bool bloqueVerde = false;

  if (pixy.ccc.numBlocks) {
    for (uint16_t i = 0; i < pixy.ccc.numBlocks; i++) {
      if (pixy.ccc.blocks[i].m_signature == ROJO_SIGNATURE) bloqueRojo = true;
      if (pixy.ccc.blocks[i].m_signature == VERDE_SIGNATURE) bloqueVerde = true;
    }
  }

  if (bloqueRojo || bloqueVerde) pixy.setLamp(1, 1);
  else pixy.setLamp(0, 0);

  Serial.print("Distancias (cm) - Frontal: ");
  Serial.print(frontal);
  Serial.print(" | Izquierda: ");
  Serial.print(izquierda);
  Serial.print(" | Derecha: ");
  Serial.print(derecha);
  Serial.print(" | Pixy: ");
  if (bloqueRojo) Serial.print("ROJO ");
  if (bloqueVerde) Serial.print("VERDE ");
  Serial.println();

  if (bloqueRojo) {
    Parar();
    delay(120);
    Serial.println("Evitando BLOQUE ROJO: Giro leve derecha.");
    Derechabloque();
    Adelante();
    motorEnMarcha = true;
    delay(180);
    return;
  }
  if (bloqueVerde) {
    Parar();
    delay(120);
    Serial.println("Evitando BLOQUE VERDE: Giro leve izquierda.");
    Izquierdabloque();
    Adelante();
    motorEnMarcha = true;
    delay(180);
    return;
  }

  if (contadorGiros >= 12) {
    if (!finalizado) {
      Serial.println("Se alcanzaron 12 giros, avanzando 1 segundo más y deteniéndose.");
      Adelante();
      delay(1000);
      Parar();
      motorEnMarcha = false;
      finalizado = true;
    }
    return;
  }

  if (!girando) {
    if (frontal != -1 && frontal > DISTANCIA_OBSTACULO_FRONTAL) {
      if (!motorEnMarcha) {
        Adelante();
        motorEnMarcha = true;
      }
      if (ahora - tiempoUltimoGiro < TIEMPO_ESPERA_GIRO) {
        Serial.println("Avanzando recto después del giro, sin girar");
      } else {
        if (izquierda != -1 && izquierda > DISTANCIA_OBSTACULO_LATERAL) {
          girando = true;
          Parar();
          motorEnMarcha = false;
          Serial.println("Giro completo izquierda (esquina)");
          Izquierda();
          girando = false;
          tiempoUltimoGiro = millis();
          Adelante();
          motorEnMarcha = true;
        } else if (derecha != -1 && derecha > DISTANCIA_OBSTACULO_LATERAL) {
          girando = true;
          Parar();
          motorEnMarcha = false;
          Serial.println("Giro completo derecha (esquina)");
          Derecha();
          girando = false;
          tiempoUltimoGiro = millis();
          Adelante();
          motorEnMarcha = true;
        }
      }
    } else if (frontal != -1 && frontal <= DISTANCIA_OBSTACULO_FRONTAL) {
      if (motorEnMarcha) {
        Parar();
        motorEnMarcha = false;
      }
      Serial.println("Obstáculo frontal detectado, detenido");
    }
  }
}

void Adelante() {
  esc.write(90);
  delay(200);
  esc.write(130);
  Serial.println("Motor en marcha hacia adelante");
}

void Parar() {
  esc.write(90);
  Serial.println("Motor detenido");
}

void Derecha() {
  esc.write(130);
  myservo.write(30);
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_MS) {
    delay(10);
  }
  myservo.write(99);
  esc.write(90);
  Serial.println("Giro derecha completo");
  contadorGiros++;
}

void Derechabloque() {
  esc.write(130);
  myservo.write(60); // Giro leve derecha (ajusta este valor según tu robot)
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_BLOQUE_MS) {
    delay(10);
  }
  myservo.write(99);
  esc.write(90);
  Serial.println("Giro leve derecha para esquivar bloque");
  // NO aumenta contadorGiros
}

void Izquierda() {
  esc.write(130);
  myservo.write(150);
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_MS) {
    delay(10);
  }
  myservo.write(99);
  esc.write(90);
  Serial.println("Giro izquierda completo");
  contadorGiros++;
}

void Izquierdabloque() {
  esc.write(130);
  myservo.write(130); // Giro leve izquierda (ajusta este valor según tu robot)
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_BLOQUE_MS) {
    delay(10);
  }
  myservo.write(99);
  esc.write(90);
  Serial.println("Giro leve izquierda para esquivar bloque");
  // NO aumenta contadorGiros
}
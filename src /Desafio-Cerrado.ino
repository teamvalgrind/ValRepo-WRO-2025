/*
  Código de Heimdall para la categoría futuros ingenieros de la WRO 2025
  Hecho por Cristobal Mogollon y Samuel Burgos
*/

#include <Wire.h>
#include <Ultrasonic.h>
#include <ESP32Servo.h>

// Pines ESP32 para sensores ultrasónicos
#define USTFRONT 13
#define USEFRONT 12
#define USTLEFT 14
#define USELEFT 27
#define USTRIGHT 26
#define USERIGHT 25

#define PIN_ESC 18
#define PIN_SERVO 19
#define PIN_BOTON 4  // Pin para botón de inicio

Ultrasonic USFront(USTFRONT, USEFRONT);
Ultrasonic USLeft(USTLEFT, USELEFT);
Ultrasonic USRight(USTRIGHT, USERIGHT);

Servo esc;
Servo myservo;

const int DISTANCIA_OBSTACULO_FRONTAL = 20;
const int DISTANCIA_OBSTACULO_LATERAL = 190;
const unsigned long DURACION_GIRO_MS = 750;
const unsigned long TIEMPO_ESPERA_GIRO = 2500; // 2500 ms

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

  pinMode(PIN_BOTON, INPUT_PULLUP);  // Botón con resistencia interna pull-up

  esc.write(90);  // ESC en posición neutra
  myservo.write(100); // Servo centrado
  delay(3000);

  Serial.println("Esperando pulsar botón para iniciar...");
}

void loop() {
  if (!programaIniciado) {
    if (digitalRead(PIN_BOTON) == LOW) {
      programaIniciado = true;
      Serial.println("Botón presionado, iniciando programa...");
      delay(500); // debounce
    }
  } else if (!finalizado) {
    docegiros();
  }
  // Si finalizado, no hace nada más
}

void docegiros() {
  unsigned long ahora = millis();

  int frontal = USFront.read();
  int izquierda = USLeft.read();
  int derecha = USRight.read();

  if (frontal == 357) frontal = -1;
  if (izquierda == 357) izquierda = -1;
  if (derecha == 357) derecha = -1;

  Serial.print("Distancias (cm) - Frontal: ");
  Serial.print(frontal);
  Serial.print(" | Izquierda: ");
  Serial.print(izquierda);
  Serial.print(" | Derecha: ");
  Serial.println(derecha);

  if (contadorGiros >= 12) {
    // Avanzar 1 segundo más y detenerse definitivamente
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
          Serial.println("Girando a la izquierda por más de 190 cm libres");
          Izquierda();
          contadorGiros++;
          girando = false;
          tiempoUltimoGiro = millis();
          Adelante();
          motorEnMarcha = true;
        } else if (derecha != -1 && derecha > DISTANCIA_OBSTACULO_LATERAL) {
          girando = true;
          Parar();
          motorEnMarcha = false;
          Serial.println("Girando a la derecha por más de 190 cm libres");
          Derecha();
          contadorGiros++;
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

void Izquierda() {
  esc.write(130);
  myservo.write(150); // Ángulo extremo derecha
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_MS) {
    delay(10);
  }
  myservo.write(99); // Centrar servo
  esc.write(90);
  Serial.println("Giro izquierda completado");
}

void Derecha() {
  esc.write(130);
  myservo.write(30); // Ángulo extremo izquierda
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_MS) {
    delay(10);
  }
  myservo.write(99); // Centrar servo
  esc.write(90);
  Serial.println("Giro derecha completado");
}

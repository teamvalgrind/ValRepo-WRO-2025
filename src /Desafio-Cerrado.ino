/*
  Código de Heimdall para la categoría futuros ingenieros de la WRO 2025
  Hecho por Cristobal Mogollon y Samuel Burgos

  El código hace que el robot al prenderse avance y de 3 vueltas a la pista
  de futuros ingenieros en la etapa abierta y al completar la cantidad de 12
  giros a la pista avanzará dos segundos más y se detiene automáticamente

  Version del codigo: FINAL

  12/06/2025
  9:04 PM
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

#define IN2 17
#define IN1 16
#define PIN_SERVO 19
#define PIN_BOTON 4  // Pin para botón de inicio

Ultrasonic USFront(USTFRONT, USEFRONT);
Ultrasonic USLeft(USTLEFT, USELEFT);
Ultrasonic USRight(USTRIGHT, USERIGHT);

Servo myservo;

const int DISTANCIA_OBSTACULO_FRONTAL = 20;
const int DISTANCIA_OBSTACULO_LATERAL = 200;
const unsigned long DURACION_GIRO_I = 1743;
const unsigned long DURACION_GIRO_D = 1543;
const unsigned long TIEMPO_ESPERA_GIRO = 3000;  // 2500 ms

bool programaIniciado = false;
bool finalizado = false;

unsigned long tiempoUltimoGiro = 0;
int contadorGiros = 0;

void setup() {
  myservo.attach(PIN_SERVO);
  Serial.begin(115200);

  pinMode(PIN_BOTON, INPUT_PULLUP);  // Botón con resistencia interna pull-up
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, LOW);     
  digitalWrite(IN2, LOW); 
  myservo.write(99);  // Servo centrado
  delay(3000);

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
      delay(1500);
      Parar();
      finalizado = true;
    }
    return;
  }

    if (frontal != -1 && frontal > DISTANCIA_OBSTACULO_FRONTAL) {
      Adelante();
      }

      if (ahora - tiempoUltimoGiro < TIEMPO_ESPERA_GIRO) {
        Serial.println("Avanzando recto después del giro, sin girar");
      } else {
        if (izquierda != -1 && izquierda > DISTANCIA_OBSTACULO_LATERAL) {
          Serial.println("Girando a la izquierda por más de 190 cm libres");
          delay(500);
          Izquierda();
          contadorGiros++;
          tiempoUltimoGiro = millis();
          Adelante();
        } else if (derecha != -1 && derecha > DISTANCIA_OBSTACULO_LATERAL) {
          Serial.println("Girando a la derecha por más de 190 cm libres");
          delay(500);
          Derecha();
          contadorGiros++;
          tiempoUltimoGiro = millis();
          Adelante();
        }
      }
    } 

void Adelante() {
  digitalWrite(IN1, HIGH);
  Serial.println("Motor en marcha hacia adelante");
}

void Parar() {
  digitalWrite(IN1, LOW);
  Serial.println("Motor detenido");
}

void Izquierda() {
  digitalWrite(IN1, HIGH);
  myservo.write(150);  
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_I) {
    delay(10);
  }
  myservo.write(97);  // Centrar servo
  digitalWrite(IN1, LOW);
  Serial.println("Giro izquierda completado");
}

void Derecha() {
  digitalWrite(IN1, HIGH);
  myservo.write(30);
  unsigned long inicio = millis();
  while (millis() - inicio < DURACION_GIRO_D) {
    delay(10);
  }
  myservo.write(100);  // Centrar servo
  digitalWrite(IN1, LOW);
  Serial.println("Giro derecha completado");
}

#include <Wire.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <Pixy2SPI_SS.h>
#include <NewPing.h>

// Pines y canales
#define IN1 16
#define IN2 17
#define MOTOR_PWM_CHANNEL 6
#define MOTOR_PWM_PIN IN1
#define PIN_SERVO 2
#define PIN_BOTON 4

#define USTLEFT 14
#define USELEFT 27
#define USTRIGHT 26
#define USERIGHT 25
#define USTFRONT 13
#define USEFRONT 12

#define MAX_DISTANCE 357
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

NewPing sensorFront(USTFRONT, USEFRONT, MAX_DISTANCE);
NewPing sensorIzq(USTLEFT, USELEFT, MAX_DISTANCE);
NewPing sensorDer(USTRIGHT, USERIGHT, MAX_DISTANCE);

Pixy2SPI_SS pixy;
Servo myservo;

// Velocidades
int velocidadNormal = 150;
int velocidadReversa = 100;

// Parámetros de control
const int ANGULO_CENTRO = 100;
const int ANGULO_MIN = 60;
const int ANGULO_MAX = 140;
const double KP = 1.5;
const double KI = 0.02;
const double KD = 0.6;
const int UMBRAL_CORRECCION = 100; // ahora 100cm
const int UMBRAL_CORRECCION_OFF = 100;
const int DISTANCIA_OBSTACULO_FRONTAL = 20;
const int LECTURAS_SENSOR = 5;

// Tiempos de acción
const unsigned long TIEMPO_GIRO_VERDE = 550;
const unsigned long TIEMPO_GIRO_ROJO = 650;
const unsigned long DELAY_RETROCESO_GIRO = 1050;
const unsigned long DELAY_RETROCESO_FINAL = 2300;
const unsigned long TIEMPO_POST_DOBLE_GIRO = 400;
const unsigned long DELAY_AVANCE_SIN_CORRECION = 1500;

// Estados
enum EstadoRobot {
  ESPERANDO,
  CENTRANDO_INICIAL,
  AVANZANDO,
  AVANZANDO_SIN_CORRECION,
  GIRANDO,
  DOBLE_GIRANDO,
  RETROCEDIENDO_GIRO,
  RETROCEDIENDO_FINAL,
  POST_DOBLE_GIRO_CORRECCION
};
EstadoRobot estadoActual = ESPERANDO;

bool programaIniciado = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoInicioAvanceSinCorreccion = 0;
uint8_t colorActual = 0;
unsigned long tiempoGiroTotal = 0;
bool modoCruce = false;
unsigned long tiempoPostCruce = 0;
int direccionGiroObstaculo = 0;
int lastCrossingDirection = 0;

// PID variables
double error = 0;
double errorPrevio = 0;
double integral = 0;
double derivada = 0;

const int UMBRAL_IZQUIERDA = 115;
const int UMBRAL_DERECHA = 130;
const int UMBRAL_Y = 15; // ¡más bajo para anticipar la esquiva!
const int ANGULO_GIRO_IZQ = 170;
const int ANGULO_GIRO_DER = 30;

bool esquivandoBloque = false;

// Función para lectura filtrada de sensores
int leerSensorFiltrado(NewPing& sensor) {
  int lecturas[LECTURAS_SENSOR];
  for (int i = 0; i < LECTURAS_SENSOR; i++) {
    lecturas[i] = sensor.ping_cm();
    delay(10);
  }
  // Ordenar lecturas (bubble sort simple)
  for (int i = 0; i < LECTURAS_SENSOR - 1; i++) {
    for (int j = 0; j < LECTURAS_SENSOR - i - 1; j++) {
      if (lecturas[j] > lecturas[j + 1]) {
        int temp = lecturas[j];
        lecturas[j] = lecturas[j + 1];
        lecturas[j + 1] = temp;
      }
    }
  }
  // Usar la mediana
  return lecturas[LECTURAS_SENSOR / 2];
}

void setup() {
  Serial.begin(115200);
  myservo.attach(PIN_SERVO);

  pinMode(PIN_BOTON, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  ledcSetup(MOTOR_PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_PWM_PIN, MOTOR_PWM_CHANNEL);
  ledcWrite(MOTOR_PWM_CHANNEL, 0);

  digitalWrite(IN2, LOW);
  myservo.write(ANGULO_CENTRO);

  SPI.begin(18, 19, 23, 5);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  if (pixy.init() == 0) {
    Serial.println("Pixy2 inicializada correctamente.");
  } else {
    Serial.println("Error al inicializar Pixy2.");
  }
  pixy.changeProg("color_connected_components");
  pixy.setLamp(1, 0);

  Serial.println("Esperando pulsar botón para iniciar...");
}

void loop() {
  if (!programaIniciado) {
    if (digitalRead(PIN_BOTON) == LOW) {
      programaIniciado = true;
      Serial.println("Programa iniciado.");
      delay(500);
      estadoActual = CENTRANDO_INICIAL;
      tiempoInicio = millis();
    }
    return;
  }

  pixy.ccc.getBlocks();

  // Lecturas filtradas de sensores
  int front = leerSensorFiltrado(sensorFront);
  int left = leerSensorFiltrado(sensorIzq);
  int right = leerSensorFiltrado(sensorDer);

  switch (estadoActual) {
    case ESPERANDO:
      break;

    case CENTRANDO_INICIAL:
      estadoActual = AVANZANDO;
      break;

    case AVANZANDO:
      // Detectar obstáculo frontal
      if (front > 0 && front <= DISTANCIA_OBSTACULO_FRONTAL) {
        manejarMotorReversa();
        if (lastCrossingDirection != 0) {
          direccionGiroObstaculo = lastCrossingDirection;
        } else if (left < right) {
          direccionGiroObstaculo = 1; lastCrossingDirection = 1;
        } else {
          direccionGiroObstaculo = 2; lastCrossingDirection = 2;
        }
        if (direccionGiroObstaculo == 1) {
          myservo.write(ANGULO_MAX);
        } else {
          myservo.write(ANGULO_MIN);
        }
        Serial.println("Obstáculo frontal: retrocediendo y girando.");
        tiempoInicio = millis();
        estadoActual = RETROCEDIENDO_GIRO;
        modoCruce = false;
        break;
      }

      // Si está en modo cruce, avanza recto sin corregir
      if (modoCruce) {
        if (millis() - tiempoPostCruce < TIEMPO_POST_DOBLE_GIRO) {
          myservo.write(ANGULO_CENTRO);
          manejarMotorAdelante();
          break;
        } else {
          modoCruce = false;
        }
      }

      // --- CORRECCIÓN LÓGICA MEJORADA ---
      // Corrige solo si ambas paredes < 100cm, y no está girando ni esquivando bloque
      if (!esquivandoBloque && estadoActual != GIRANDO && estadoActual != DOBLE_GIRANDO &&
          left < UMBRAL_CORRECCION && right < UMBRAL_CORRECCION) {
        error = left - right;
        integral += error;
        derivada = error - errorPrevio;
        int anguloServo = ANGULO_CENTRO + KP * error + KI * integral + KD * derivada;
        anguloServo = constrain(anguloServo, ANGULO_MIN, ANGULO_MAX);
        myservo.write(anguloServo);
        errorPrevio = error;
        manejarMotorAdelante();
      }
      // Si no, va recto
      else {
        myservo.write(ANGULO_CENTRO);
        manejarMotorAdelante();
      }

      // Pixy2: detección de bloques (más anticipada)
      if (pixy.ccc.numBlocks > 0) {
        uint8_t sig = pixy.ccc.blocks[0].m_signature;
        int x = pixy.ccc.blocks[0].m_x;
        int y = pixy.ccc.blocks[0].m_y;
        if (y > UMBRAL_Y) { // ahora UMBRAL_Y=15
          esquivandoBloque = true;
          detectarBloque(sig, x);
        }
      } else {
        esquivandoBloque = false;
      }
      break;

    case AVANZANDO_SIN_CORRECION:
      manejarMotorAdelante();
      myservo.write(ANGULO_CENTRO);
      if (millis() - tiempoInicioAvanceSinCorreccion >= DELAY_AVANCE_SIN_CORRECION) {
        estadoActual = AVANZANDO;
      }
      break;

    case RETROCEDIENDO_GIRO:
      manejarMotorReversa();
      if (direccionGiroObstaculo == 1) {
        myservo.write(ANGULO_MAX);
      } else {
        myservo.write(ANGULO_MIN);
      }
      if (millis() - tiempoInicio >= DELAY_RETROCESO_GIRO) {
        myservo.write(ANGULO_CENTRO);
        tiempoInicio = millis();
        estadoActual = RETROCEDIENDO_FINAL;
      }
      break;

    case RETROCEDIENDO_FINAL:
      manejarMotorReversa();
      myservo.write(ANGULO_CENTRO);
      if (millis() - tiempoInicio >= DELAY_RETROCESO_FINAL) {
        tiempoInicioAvanceSinCorreccion = millis();
        estadoActual = AVANZANDO_SIN_CORRECION;
      }
      break;

    case POST_DOBLE_GIRO_CORRECCION:
      manejarMotorAdelante();
      myservo.write(ANGULO_CENTRO);
      if (millis() - tiempoInicio >= TIEMPO_POST_DOBLE_GIRO) {
        estadoActual = AVANZANDO;
        myservo.write(ANGULO_CENTRO);
        modoCruce = true;
        tiempoPostCruce = millis();
      }
      break;

    case GIRANDO:
      manejarMotorAdelante();
      if (colorActual == 1) {
        myservo.write(ANGULO_GIRO_IZQ);
        tiempoGiroTotal = TIEMPO_GIRO_VERDE;
        if (millis() - tiempoInicio >= tiempoGiroTotal) {
          tiempoInicio = millis();
          estadoActual = DOBLE_GIRANDO;
        }
      } else if (colorActual == 2) {
        myservo.write(ANGULO_GIRO_DER);
        tiempoGiroTotal = TIEMPO_GIRO_ROJO;
        if (millis() - tiempoInicio >= tiempoGiroTotal) {
          tiempoInicio = millis();
          estadoActual = DOBLE_GIRANDO;
        }
      }
      break;

    case DOBLE_GIRANDO:
      manejarMotorAdelante();
      if (colorActual == 1) {
        myservo.write(ANGULO_GIRO_DER);
        if (millis() - tiempoInicio >= TIEMPO_GIRO_VERDE) {
          tiempoInicio = millis();
          estadoActual = POST_DOBLE_GIRO_CORRECCION;
        }
      } else if (colorActual == 2) {
        myservo.write(ANGULO_GIRO_IZQ);
        if (millis() - tiempoInicio >= TIEMPO_GIRO_ROJO) {
          tiempoInicio = millis();
          estadoActual = POST_DOBLE_GIRO_CORRECCION;
        }
      }
      break;
  }
}

void detectarBloque(uint8_t color, int x) {
  if (color == 1) { // Verde
    if (x >= UMBRAL_IZQUIERDA && x <= UMBRAL_DERECHA) {
      iniciarGiroParcial(1);
    } else if (x < UMBRAL_IZQUIERDA) {
      iniciarGiro(1);
    }
  } else if (color == 2) { // Rojo
    if (x >= UMBRAL_IZQUIERDA && x <= UMBRAL_DERECHA) {
      iniciarGiroParcial(2);
    } else if (x > UMBRAL_DERECHA) {
      iniciarGiro(2);
    }
  }
}

void iniciarGiro(uint8_t color) {
  colorActual = color;
  tiempoInicio = millis();
  estadoActual = GIRANDO;
}

void iniciarGiroParcial(uint8_t color) {
  colorActual = color;
  tiempoInicio = millis();
  estadoActual = GIRANDO;
}

void manejarMotorAdelante() {
  ledcWrite(MOTOR_PWM_CHANNEL, velocidadNormal);
  digitalWrite(IN2, LOW);
}

void manejarMotorReversa() {
  ledcWrite(MOTOR_PWM_CHANNEL, velocidadReversa);
  digitalWrite(IN2, HIGH);
}
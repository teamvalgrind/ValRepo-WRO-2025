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
#define PIN_BOTON 15

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
int velocidadGiro = 150;

// Parámetros de control
const int ANGULO_CENTRO = 98;
const int ANGULO_MAX = 128;    // Máximo giro a la DERECHA (98 + 30)
const int ANGULO_MIN = 68;     // Máximo giro a la IZQUIERDA (98 - 30)

// Parámetros de esquiva (ajustables)
const int ANGULO_ESQUIVA_PRIMARIO = 25;   // Grados de giro para la primera parte de la esquiva
const int ANGULO_ESQUIVA_SECUNDARIO = 20; // Grados de giro para la segunda parte de la esquiva
const unsigned long TIEMPO_ESQUIVA_PRIMARIA = 730;   // Tiempo primera parte de la esquiva
const unsigned long TIEMPO_RECTA_ENTRE_GIROS = 100;  // Tiempo yendo recto entre giros
const unsigned long TIEMPO_ESQUIVA_SECUNDARIA = 650; // Tiempo segunda parte de la esquiva

// Tiempos de maniobras (en milisegundos) - DIFERENTES PARA CADA BLOQUE
const unsigned long TIEMPO_ESQUIVA_VERDE = TIEMPO_ESQUIVA_PRIMARIA + TIEMPO_RECTA_ENTRE_GIROS + TIEMPO_ESQUIVA_SECUNDARIA;
const unsigned long TIEMPO_ESQUIVA_ROJO = TIEMPO_ESQUIVA_PRIMARIA + TIEMPO_RECTA_ENTRE_GIROS + TIEMPO_ESQUIVA_SECUNDARIA;

// Tiempos POST-ESQUIVA DIFERENTES para cada bloque
const unsigned long TIEMPO_POST_ESQUIVA_VERDE = 100;   // Tiempo para avanzar después de la esquiva verde
const unsigned long TIEMPO_POST_ESQUIVA_ROJO = 150;    // Tiempo para avanzar después de la esquiva rojo

// Tiempos de GIRO DIFERENTES para cada bloque
const unsigned long TIEMPO_GIRO_DERECHA_VERDE = 850;   // Tiempo para girar a la derecha después de esquiva verde
const unsigned long TIEMPO_GIRO_IZQUIERDA_VERDE = 630; // Tiempo para girar a la izquierda después de esquiva verde
const unsigned long TIEMPO_GIRO_IZQUIERDA_ROJO = 690;  // Tiempo para girar a la izquierda después de esquiva rojo
const unsigned long TIEMPO_GIRO_DERECHA_ROJO = 500;    // Tiempo para girar a la derecha después de esquiva rojo

const unsigned long TIEMPO_RETROCESO = 1000;
const unsigned long TIEMPO_DETENCION = 3000;
const unsigned long TIEMPO_RETROCESO_FINAL = 3000;

// Umbrales de detección de Pixy INDEPENDIENTES para cada bloque
const int UMBRAL_TAMANO_BLOQUE_VERDE = 2500;  // Umbral específico para bloque verde
const int UMBRAL_TAMANO_BLOQUE_ROJO = 100;   // Umbral específico para bloque rojo
const int OBSTACULO_FRONTAL = 26;

// Estados del robot
enum Estado {
  DETENIDO,
  AVANZAR,
  ESQUIVAR_VERDE_IZQ,
  ESQUIVAR_ROJO_DER,
  POST_ESQUIVA_VERDE,  // Secuencia después de esquivar verde
  POST_ESQUIVA_ROJO,   // Secuencia después de esquivar rojo
  GIRO_SERVO,
  RETROCEDER_CON_GIRO,
  RETROCEDER_FINAL,
  REAJUSTAR_POSICION
};

Estado estadoActual = DETENIDO;
bool bloqueVerdeDetectado = false;
bool bloqueRojoDetectado = false;
bool bloqueVerdeCercano = false;
bool bloqueRojoCercano = false;
int posicionBloqueX = 0;
int ladoMasLibre = 0;

// Variables para el control de movimiento por tiempo
unsigned long tiempoInicioManiobra = 0;
unsigned long tiempoFaseActual = 0;

void setup() {
  Serial.begin(115200);
  
  // Configurar pines de motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  ledcSetup(MOTOR_PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_PWM_PIN, MOTOR_PWM_CHANNEL);
  
  // Configurar servo
  myservo.attach(PIN_SERVO);
  myservo.write(ANGULO_CENTRO);
  
  // Configurar botón
  pinMode(PIN_BOTON, INPUT_PULLUP);
  
  // Inicializar Pixy
  pixy.init();
  
  // Encender luces de la Pixy
  pixy.setLamp(1, 1);
  
  Serial.println("Robot inicializado. Presione el botón para comenzar.");
}

void loop() {
  // Leer estado del botón
  if (digitalRead(PIN_BOTON) == LOW && estadoActual == DETENIDO) {
    estadoActual = AVANZAR;
    delay(500);
    Serial.println("Iniciando movimiento");
  }
  
  // Leer sensores ultrasónicos
  int distanciaFrontal = sensorFront.ping_cm();
  int distanciaIzquierda = sensorIzq.ping_cm();
  int distanciaDerecha = sensorDer.ping_cm();
  
  // Leer datos de Pixy (pero no detener la maniobra if ya comenzó)
  if (estadoActual != ESQUIVAR_VERDE_IZQ && estadoActual != ESQUIVAR_ROJO_DER && 
      estadoActual != POST_ESQUIVA_VERDE && estadoActual != POST_ESQUIVA_ROJO) {
    leerPixy();
  }
  
  // Mostrar información de depuración
  if (millis() % 1000 < 50) {
    Serial.print("Estado: ");
    Serial.print(estadoActual);
    Serial.print(" | Frontal: ");
    Serial.print(distanciaFrontal);
    Serial.print("cm | Izq: ");
    Serial.print(distanciaIzquierda);
    Serial.print("cm | Der: ");
    Serial.print(distanciaDerecha);
    Serial.print("cm | Verde: ");
    Serial.print(bloqueVerdeDetectado);
    Serial.print(" | Rojo: ");
    Serial.print(bloqueRojoDetectado);
    Serial.print(" | VerdeCerca: ");
    Serial.print(bloqueVerdeCercano);
    Serial.print(" | RojoCerca: ");
    Serial.print(bloqueRojoCercano);
    Serial.print(" | PosX: ");
    Serial.print(posicionBloqueX);
    Serial.print(" | Lado libre: ");
    Serial.println(ladoMasLibre);
  }
  
  // Máquina de estados del robot
  switch (estadoActual) {
    case DETENIDO:
      detenerRobot();
      break;
      
    case AVANZAR:
      avanzar();
      myservo.write(ANGULO_CENTRO);
      
      // Verificar obstáculos y tomar decisiones
      if (distanciaFrontal > 0 && distanciaFrontal < OBSTACULO_FRONTAL) {
        if (!bloqueVerdeDetectado && !bloqueRojoDetectado) {
          if (distanciaIzquierda > distanciaDerecha + 5) {
            ladoMasLibre = 1;
          } else if (distanciaDerecha > distanciaIzquierda + 5) {
            ladoMasLibre = -1;
          } 
          
          estadoActual = GIRO_SERVO;
          tiempoInicioManiobra = millis();
          Serial.println("Obstáculo frontal detectado, girando servo");
        }
      }
      
      // Comportamiento según color detectado
      if (bloqueVerdeDetectado && bloqueVerdeCercano) {
        if (posicionBloqueX == 1) {
          Serial.println("Verde detectado a la derecha, sigo recto");
        } else if (posicionBloqueX == -1) {
          estadoActual = ESQUIVAR_VERDE_IZQ;
          tiempoInicioManiobra = millis();
          Serial.println("Verde detectado a la izquierda, iniciando esquiva");
        }
      } else if (bloqueRojoDetectado && bloqueRojoCercano) {
        if (posicionBloqueX == -1) {
          Serial.println("Rojo detectado a la izquierda, sigo recto");
        } else if (posicionBloqueX == 1) {
          estadoActual = ESQUIVAR_ROJO_DER;
          tiempoInicioManiobra = millis();
          Serial.println("Rojo detectado a la derecha, iniciando esquiva");
        }
      }
      break;
      
    case ESQUIVAR_VERDE_IZQ:
      // Esquiva para bloque verde a la izquierda
      tiempoFaseActual = millis() - tiempoInicioManiobra;
      
      if (tiempoFaseActual < TIEMPO_ESQUIVA_PRIMARIA) {
        // Primera fase: girar a la derecha para esquivar
        myservo.write(ANGULO_CENTRO + ANGULO_ESQUIVA_PRIMARIO);
        avanzar();
        Serial.println("Fase 1: Esquivando a la derecha");
      } else if (tiempoFaseActual < TIEMPO_ESQUIVA_PRIMARIA + TIEMPO_RECTA_ENTRE_GIROS) {
        // Segunda fase: centrar y avanzar recto
        myservo.write(ANGULO_CENTRO);
        avanzar();
        Serial.println("Fase 2: Avanzando recto");
      } else if (tiempoFaseActual < TIEMPO_ESQUIVA_VERDE) {
        // Tercera fase: girar a la izquierda para re-centrar
        myservo.write(ANGULO_CENTRO - ANGULO_ESQUIVA_SECUNDARIO);
        avanzar();
        Serial.println("Fase 3: Re-centrando a la izquierda");
      } else {
        // Finalizar maniobra e iniciar secuencia post-esquiva
        estadoActual = POST_ESQUIVA_VERDE;
        tiempoInicioManiobra = millis();
        Serial.println("Esquiva verde completada, iniciando secuencia post-esquiva");
      }
      break;
      
    case POST_ESQUIVA_VERDE:
      // Secuencia adicional después de esquivar el bloque verde (TIEMPOS ESPECÍFICOS PARA VERDE)
      tiempoFaseActual = millis() - tiempoInicioManiobra;
      
      if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_VERDE) {
        // Avanzar recto (tiempo específico para verde)
        myservo.write(ANGULO_CENTRO);
        avanzar();
        Serial.println("Post-esquiva verde: Avanzando recto");
      } else if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_VERDE + TIEMPO_GIRO_DERECHA_VERDE) {
        // Girar a la derecha (tiempo específico para verde)
        myservo.write(ANGULO_CENTRO - ANGULO_ESQUIVA_PRIMARIO);
        avanzar();
        Serial.println("Post-esquiva verde: Girando a la derecha");
      } else if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_VERDE + TIEMPO_GIRO_DERECHA_VERDE + TIEMPO_RECTA_ENTRE_GIROS) {
        // Centrar el servo
        myservo.write(ANGULO_CENTRO);
        avanzar();
        Serial.println("Post-esquiva verde: Centrando servo");
      } else if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_VERDE + TIEMPO_GIRO_DERECHA_VERDE + TIEMPO_RECTA_ENTRE_GIROS + TIEMPO_GIRO_IZQUIERDA_VERDE) {
        // Girar a la izquierda (tiempo específico para verde)
        myservo.write(ANGULO_CENTRO + ANGULO_ESQUIVA_PRIMARIO);
        avanzar();
        Serial.println("Post-esquiva verde: Girando a la izquierda");
      } else {
        // Finalizar secuencia y volver a avanzar normalmente
        myservo.write(ANGULO_CENTRO);
        estadoActual = AVANZAR;
        Serial.println("Secuencia post-esquiva verde completada, avanzando normalmente");
      }
      break;
      
    case ESQUIVAR_ROJO_DER:
      // Esquiva para bloque rojo a la derecha
      tiempoFaseActual = millis() - tiempoInicioManiobra;
      
      if (tiempoFaseActual < TIEMPO_ESQUIVA_PRIMARIA) {
        // Primera fase: girar a la izquierda para esquivar
        myservo.write(ANGULO_CENTRO - ANGULO_ESQUIVA_PRIMARIO);
        avanzar();
        Serial.println("Fase 1: Esquivando a la izquierda");
      } else if (tiempoFaseActual < TIEMPO_ESQUIVA_PRIMARIA + TIEMPO_RECTA_ENTRE_GIROS) {
        // Segunda fase: centrar y avanzar recto
        myservo.write(ANGULO_CENTRO);
        avanzar();
        Serial.println("Fase 2: Avanzando recto");
      } else if (tiempoFaseActual < TIEMPO_ESQUIVA_ROJO) {
        // Tercera fase: girar a la derecha para re-centrar
        myservo.write(ANGULO_CENTRO + ANGULO_ESQUIVA_SECUNDARIO);
        avanzar();
        Serial.println("Fase 3: Re-centrando a la derecha");
      } else {
        // Finalizar maniobra e iniciar secuencia post-esquiva para rojo
        estadoActual = POST_ESQUIVA_ROJO;
        tiempoInicioManiobra = millis();
        Serial.println("Esquiva rojo completada, iniciando secuencia post-esquiva");
      }
      break;
      
    case POST_ESQUIVA_ROJO:
      // Secuencia adicional después de esquivar el bloque rojo (TIEMPOS ESPECÍFICOS PARA ROJO)
      tiempoFaseActual = millis() - tiempoInicioManiobra;
      
      if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_ROJO) {
        // Avanzar recto (tiempo específico para rojo)
        myservo.write(ANGULO_CENTRO);
        avanzar();
        Serial.println("Post-esquiva rojo: Avanzando recto");
      } else if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_ROJO + TIEMPO_GIRO_IZQUIERDA_ROJO) {
        // Girar a la izquierda (tiempo específico para rojo)
        myservo.write(ANGULO_CENTRO + ANGULO_ESQUIVA_PRIMARIO);
        avanzar();
        Serial.println("Post-esquiva rojo: Girando a la izquierda");
      } else if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_ROJO + TIEMPO_GIRO_IZQUIERDA_ROJO + TIEMPO_RECTA_ENTRE_GIROS) {
        // Centrar el servo
        myservo.write(ANGULO_CENTRO);
        avanzar();
        Serial.println("Post-esquiva rojo: Centrando servo");
      } else if (tiempoFaseActual < TIEMPO_POST_ESQUIVA_ROJO + TIEMPO_GIRO_IZQUIERDA_ROJO + TIEMPO_RECTA_ENTRE_GIROS + TIEMPO_GIRO_DERECHA_ROJO) {
        // Girar a la derecha (tiempo específico para rojo)
        myservo.write(ANGULO_CENTRO - ANGULO_ESQUIVA_PRIMARIO);
        avanzar();
        Serial.println("Post-esquiva rojo: Girando a la derecha");
      } else {
        // Finalizar secuencia y volver a avanzar normalmente
        myservo.write(ANGULO_CENTRO);
        estadoActual = AVANZAR;
        Serial.println("Secuencia post-esquiva rojo completada, avanzando normalmente");
      }
      break;
      
    case GIRO_SERVO:
      if (ladoMasLibre == -1) {
        myservo.write(ANGULO_MAX); // Girar a la DERECHA (contrario a izquierda libre)
        Serial.println("Servo girado a derecha (contrario a izquierda libre)");
      } else {
        myservo.write(ANGULO_MIN); // Girar a la IZQUIERDA (contrario a derecha libre)
        Serial.println("Servo girado a izquierda (contrario a derecha libre)");
      }
      
      estadoActual = RETROCEDER_CON_GIRO;
      tiempoInicioManiobra = millis();
      break;
      
    case RETROCEDER_CON_GIRO:
      tiempoFaseActual = millis() - tiempoInicioManiobra;
      
      if (tiempoFaseActual < TIEMPO_RETROCESO) {
        retroceder();
        Serial.println("Retrocediendo con servo girado");
      } else {
        myservo.write(ANGULO_CENTRO);
        estadoActual = RETROCEDER_FINAL;
        tiempoInicioManiobra = millis();
        Serial.println("Centrando servo y retrocediendo más");
      }
      break;
      
    case RETROCEDER_FINAL:
      tiempoFaseActual = millis() - tiempoInicioManiobra;
      
      if (tiempoFaseActual < TIEMPO_RETROCESO_FINAL) {
        retroceder();
        Serial.println("Retrocediendo final hacia la pared");
      } else {
        estadoActual = AVANZAR;
        Serial.println("Retroceso finalizado, avanzando");
      }
      break;
      
    case REAJUSTAR_POSICION:
      estadoActual = AVANZAR;
      break;
  }
  
  delay(50);
}

void leerPixy() {
  static unsigned long ultimaLectura = 0;
  
  if (millis() - ultimaLectura > 100) {
    ultimaLectura = millis();
    
    pixy.ccc.getBlocks();
    
    bloqueVerdeDetectado = false;
    bloqueRojoDetectado = false;
    bloqueVerdeCercano = false;
    bloqueRojoCercano = false;
    posicionBloqueX = 0;
    
    if (pixy.ccc.numBlocks) {
      for (int i = 0; i < pixy.ccc.numBlocks; i++) {
        if (pixy.ccc.blocks[i].m_signature == 1) {
          bloqueVerdeDetectado = true;
          if (pixy.ccc.blocks[i].m_x < pixy.frameWidth / 2) {
            posicionBloqueX = -1;
          } else {
            posicionBloqueX = 1;
          }
          // Umbral específico para verde
          if (pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height > UMBRAL_TAMANO_BLOQUE_VERDE) {
            bloqueVerdeCercano = true;
          }
          break;
        }
        else if (pixy.ccc.blocks[i].m_signature == 2) {
          bloqueRojoDetectado = true;
          if (pixy.ccc.blocks[i].m_x < pixy.frameWidth / 2) {
            posicionBloqueX = -1;
          } else {
            posicionBloqueX = 1;
          }
          // Umbral específico para rojo
          if (pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height > UMBRAL_TAMANO_BLOQUE_ROJO) {
            bloqueRojoCercano = true;
          }
          break;
        }
      }
    }
  }
}

void avanzar() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(MOTOR_PWM_CHANNEL, velocidadNormal);
}

void retroceder() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(MOTOR_PWM_CHANNEL, velocidadReversa);
}

void detenerRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  ledcWrite(MOTOR_PWM_CHANNEL, 0);
}

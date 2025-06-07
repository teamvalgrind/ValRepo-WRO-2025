/*
  Código de Heimdall para la categoría futuros ingenieros de la WRO 2025
  Hecho por Cristobal Mogollon y Samuel Burgos

  El código hace que el robot al prenderse avance y de 3 vueltas a la pista
  de futuros ingenieros en la etapa abierta y al completar la cantidad de 12
  giros a la pista avanzará dos segundos más y se detiene automáticamente

  Version del codigo: 4
  En esta version del codigo se implementa el uso del giroscopio para mejorar
  el sistema de conteo de vueltas a comparacion de la V3 que solo utilizaba 
  el acelerometro del giroscopio para saber si habia girado, al haber 
  implementado esto asegura mayor precision al momento del conteo de vueltas

 
*/

#include <Wire.h>        // Librería para I2C
#include <Ultrasonic.h>
#include <Servo.h>
#include <Adafruit_BNO08x.h> // Librería para el BNO080/BNO085

#define USTFRONT 23
#define USEFRONT 22
#define USTRIGHT 45
#define USERIGHT 44
#define USTLEFT 31
#define USELEFT 30

Ultrasonic USFront(USTFRONT, USEFRONT);
Ultrasonic USLeft(USTLEFT, USELEFT);
Ultrasonic USRight(USTRIGHT, USERIGHT);

const int DISTANCIA_OBSTACULO_FRONTAL = 15; // Distancia en cm para el sensor frontal
const int DISTANCIA_OBSTACULO_LATERAL = 50; // Distancia en cm para los sensores laterales

Servo esc;
Servo myservo;

int pos = 85;
bool motorEnMarcha = false;
int contadorVueltas = 0;         // Contador de giros (12 giros = 3 vueltas de 4 giros de 90°)
bool giroDetectado = false;       // (No se usa en este método, pero puedes quitarla si quieres)
bool robotDetenido = false;       // Variable para detener el robot después de 12 giros

// BNO085
Adafruit_BNO08x bno08x;
#define BNO08X_RESET -1           // Si no usas el pin de reset
sh2_SensorValue sensorValue;

// Variables para la detección de giros con el giroscopio
float anguloAcumuladoZ = 0.0;
unsigned long tiempoAnterior = 0;

void setup() {
  Wire.begin();                    // Inicializa la comunicación I2C para el BNO085 en Arduino Mega
  esc.attach(9, 1000, 2000);
  myservo.attach(11);
  Serial.begin(115200);

  // Inicializar el BNO085
  if (!bno08x.begin_I2C(0x4B)) {
    Serial.println("¡No se pudo iniciar el BNO08x!");
    while (1) delay(1);
  }
  bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED, 10000); // Reporte cada 10ms

  // Armar el ESC al iniciar
  esc.write(90);
  delay(3000);

  tiempoAnterior = millis();
}

void loop() {
  doceVueltas();
}

void doceVueltas() {
  if (robotDetenido) {
    // Robot detenido, no hacer nada más
    return;
  }

  int frontal = USFront.read();
  int izquierda = USLeft.read();
  int derecha = USRight.read();

  // Leer datos del giroscopio del BNO085 y acumular el ángulo girado en el eje Z
  unsigned long tiempoActual = millis();
  float deltaTime = (tiempoActual - tiempoAnterior) / 1000.0; // segundos
  tiempoAnterior = tiempoActual;

  if (bno08x.getSensorEvent(&sensorValue)) {
    if (sensorValue.sensorId == SH2_GYROSCOPE_CALIBRATED) {
      float gyroZ = sensorValue.un.gyroscope.z * 57.3; // Convertir a dps
      anguloAcumuladoZ += gyroZ * deltaTime; // Acumula el ángulo en el eje Z (con signo)
    }
  }

  // Si el ángulo acumulado supera +89° o -89°, cuenta como un giro
  if (abs(anguloAcumuladoZ) > 89) {
    contadorVueltas++;
    // (Opcional) Muestra el sentido del giro
    if (anguloAcumuladoZ > 0) {
      Serial.print("Giro a la derecha detectado. ");
    } else {
      Serial.print("Giro a la izquierda detectado. ");
    }
    anguloAcumuladoZ = 0; // Resetea el acumulador
    Serial.print("Giros: ");
    Serial.println(contadorVueltas);
  }

  Serial.print("Frontal: "); Serial.print(frontal);
  Serial.print("cm | Izq: "); Serial.print(izquierda);
  Serial.print("cm | Der: "); Serial.print(derecha);
  Serial.print("cm | Ángulo acumulado Z: ");
  Serial.println(anguloAcumuladoZ);

  if (frontal > DISTANCIA_OBSTACULO_FRONTAL) {
    if (!motorEnMarcha) {
      Adelante();
      motorEnMarcha = true;
    }
  } else {
    if (motorEnMarcha) {
      Parar();
      motorEnMarcha = false;
    }

    if (izquierda > DISTANCIA_OBSTACULO_LATERAL && izquierda > derecha) {
      Izquierda();
    } else if (derecha > DISTANCIA_OBSTACULO_LATERAL && derecha > izquierda) {
      Derecha();
    } else {
      Atras();
      delay(500);
      Parar();
    }
    delay(1000);
    myservo.write(90);
  }

  // Si contadorVueltas llega a 12, avanzar 2 segundos y parar
  if (contadorVueltas >= 12) {
    Adelante(); // Avanzamos
    delay(2000);  // Esperamos 2 segundos
    Parar();    // Detenemos el motor
    Serial.println("¡Completados 12 giros!");
    robotDetenido = true;  // Bloquea para que no siga ejecutando
  }
}

void Adelante(){
  Serial.println("Armando ESC...");
  esc.write(90);     // Pulso mínimo para armar ESC
  delay(10);         // Espera breve para armar

  Serial.println("Aumentando velocidad...");
  for (int speed = 90; speed <= 130; speed += 10) {
    esc.write(speed);
    Serial.print("Velocidad: ");
    Serial.println(speed);
    delay(250);
  }
  
  Serial.println("Manteniendo velocidad fija");
  esc.write(130);     // Mantiene velocidad fija
}

void Atras(){
  Serial.println("Aumentando velocidad hacia atrás...");
  esc.write(90);     // Pulso mínimo para armar ESC
  delay(10);
  for (int speed = 90; speed >= 0; speed -= 50) { 
    esc.write(speed);
    Serial.print("Velocidad atrás: ");
    Serial.println(speed);
    delay(200);
  }
  Serial.println("Manteniendo velocidad fija hacia atrás");
  esc.write(0);      // Mantiene velocidad fija hacia atrás
}

void Parar() {
  Serial.println("Deteniendo motor...");
  esc.write(90);  // Pulso mínimo para detener el motor (igual que armar)
}

void Derecha() {
  Serial.println("Motor en marcha y girando a la derecha gradualmente...");

  esc.write(90);      // Armar motor
  delay(10);

  esc.write(130);     // Velocidad fija

  for (int ang = 85; ang <= 180; ang++) {
    myservo.write(ang);
    Serial.print("Ángulo servo: ");
    Serial.println(ang);
    delay(1);
  }

  myservo.write(90);
  esc.write(90);
  Serial.println("Giro completado");
}

void Izquierda() {
  Serial.println("Motor en marcha y girando a la Izquierda gradualmente...");

  esc.write(90);      // Armar motor
  delay(10);

  esc.write(130);     // Velocidad fija

  for (int ang = 85; ang >= 0; ang--) {
    myservo.write(ang);
    Serial.print("Ángulo servo: ");
    Serial.println(ang);
    delay(1);
  }

  myservo.write(90);
  esc.write(90);
  Serial.println("Giro completado");
}

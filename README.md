
---

# Equipo [Team ValGrind] – WRO 2025

<img src="https://github.com/damuec/ValRepo1/assets/logo-equipo.png" alt="Logo del equipo" width="200"/>

---

##  Índice

1. [Nuestro Equipo](#nuestro-equipo)
    - [Integrantes](#integrantes)
2. [Nuestro Objetivo](#nuestro-objetivo)
3. [El Robot](#el-robot)
    - [Apartado Mecánico](#apartado-mecánico)
        - [Movilidad](#movilidad)
        - [Chasis e Impresión 3D](#chasis-e-impresión-3d)
    - [Apartado Electrónico](#apartado-electrónico)
        - [Baterías](#baterías)
        - [Sensores Ultrasónicos](#sensores-ultrasónicos)
        - [Conexiones y Circuitos](#conexiones-y-circuitos)
        - [PCB](#pcb)
        - [Microcontroladores](#microcontroladores)
    - [Apartado Programático](#apartado-programático)
        - [Código por Componente](#código-por-componente)
        - [Diagramas de Flujo](#diagramas-de-flujo)
        - [Algoritmos y Teoría](#algoritmos-y-teoría)
        - [Compiladores y Comunicación](#compiladores-y-comunicación)
4. [Recursos para Hacer el Robot](#recursos-para-hacer-el-robot)
5. [Licencia](#licencia)

---

## Nuestro Equipo

<img src="https://github.com/damuec/ValRepo1/assets/foto-equipo.jpg" alt="Foto del equipo" width="400"/>

Somos **[Team ValGrind]**, entusiastas de la robótica y la innovación. Representamos a nuestra institución en la fase regional de la **WRO 2025**, ¡con la meta de clasificar a la nacional!

-  [Institución]: [https://www.instagram.com/colegiosaltoangel]
-  [Instagram Oficial]:
(https://instagram.com/team_valgrind)
-  [Contáctanos]: [valgrind2025@gmail.com](mailto: valgrind2025@gmail.com)

---

## Nuestro Objetivo

Diseñar, construir y programar un robot autónomo capaz de superar los desafíos de la WRO 2025, utilizando innovación y trabajo en equipo para lograr un desempeño sobresaliente en la etapa regional y avanzar a la nacional. Para llegar a esto, hemos pasado los últimos meses diseñando, construyendo, armando y programando nuestro más nuevo proyecto "Heimdall" 

---

## El Robot

### Apartado Mecánico

#### Movilidad

Nuestro robot emplea un sistema de tracción diferencial, ofreciendo maniobrabilidad precisa para enfrentar los retos del campo de competencia. El sistema de cruces se realiza mediante un servo que ajusta la dirección del robot en intersecciones críticas.

#### Chasis e Impresión 3D

El chasis fue diseñado en software CAD y fabricado mediante impresión 3D en PLA, optimizando peso, resistencia y estética.  
![Chasis 3D](https://github.com/damuec/ValRepo1/assets/chasis-3d.png)

---

### Apartado Electrónico

#### Baterías

Utilizamos baterías LiPo de 7.4V para asegurar la autonomía y potencia necesarias durante la competencia.

#### Sensores Ultrasónicos

El robot cuenta con múltiples sensores ultrasónicos ubicados estratégicamente para la detección de obstáculos y el cálculo de distancias, permitiendo navegación autónoma y segura.

#### Conexiones y Circuitos

Todos los módulos están conectados en un circuito organizado, minimizando interferencias y facilitando el mantenimiento.  
[Ver diagrama del circuito](./docs/diagrama-electronico.png)


#### Microcontroladores

El corazón del robot es un **Arduino Mega 2560**, que coordina los sensores ultrasónicos, el servo, los motores y la lógica de navegación.

---

### Apartado Programático

#### Código por Componente

// Incluyendo liberias al codigo
#include <Ultrasonic.h>
#include <Servo.h>

// Definición de pines para los sensores ultrasónicos
#define USTFRONT 21
#define USEFRONT 20
#define USTRIGHT 45
#define USERIGHT 44
#define USTLEFT 31
#define USELEFT 30

// Definicion de pines para el motor
#define IN1 5
#define IN2 4

Servo myservo;
int pos = 85;

// Creación de objetos Ultrasonic para cada sensor
Ultrasonic USFront(USTFRONT, USEFRONT);
Ultrasonic USLeft(USTLEFT, USELEFT);
Ultrasonic USRight(USTRIGHT, USERIGHT);

// Arreglos para almacenar posiciones y distancias
int firstPosition[3];
int lastPosition[3];
int currentPosition[3];
int tempPosition[3];

void setup() {
  Serial.begin(9600);  // Inicializa comunicación serial a 9600 baudios
  delay(1000);         // Pequeña pausa para estabilizar

  // Configurando los pines del motor como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Leer distancias iniciales y guardar en firstPosition y lastPosition
  readDistance(firstPosition);
  readDistance(lastPosition);

  // Pin para el servo
  myservo.attach(9);
  // Posicion inicial del servo
  myservo.write(80);
}

void loop() {
  // Actualiza las distancias actuales
  readDistance(currentPosition);
  // Imprime las distancias en centímetros
  printDistance(currentPosition);
  delay(1000);
  Derecha();

}

// Función para leer las distancias de los sensores y guardarlas en el arreglo
void readDistance(int* arreglo) {
  arreglo[0] = USFront.read();
  arreglo[1] = USLeft.read();
  arreglo[2] = USRight.read();
}

// Función para imprimir las distancias en cm por el Monitor Serial
void printDistance(int* arreglo) {
  for (byte i = 0; i < 3; i++) {
    Serial.print(arreglo[i]);
    Serial.print(" cm    ");
  }
  Serial.println();
}


// Funcion para que avance
void Adelante(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  myservo.write(85);
}


// Funcion para que retroceda
void Atras(){
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  myservo.write(85);
}


// Funcion para que gire hacia la izquierda
void Izquierda(){
  digitalWrite(IN1, HIGH);
 for (pos = 85; pos <= 0; pos -= 1) {
    myservo.write(pos);             
    delay(50);                     
  }
}

// Funcion para que gire hacia la derecha
void Derecha(){
  digitalWrite(IN1, HIGH);
 for (pos = 85; pos <= 180; pos += 1) {
    myservo.write(pos);             
    delay(30);                     
  }
}

void Parar(){
  digitalWrite(IN2, LOW);
  digitalWrite(IN1, LOW);
  myservo.write(85);
}

void tresvueltas(){
}

#### Diagramas de Flujo

![Diagrama de Flujo](./docs/diagrama-flujo.png)

#### Algoritmos y Teoría

Aplicamos algoritmos de evasión de obstáculos, control PID para estabilizar la trayectoria y lógica para detección y toma de decisiones en cruces. La interacción entre sensores y actuadores está cuidadosamente programada para garantizar eficiencia y precisión.

#### Compiladores y Comunicación

- **Lenguaje principal:** C++ (Arduino IDE)
- **Compilador:** [Arduino IDE](https://www.arduino.cc/en/software)
- **Comunicación entre módulos:** Bus I2C y UART

---

## 🛒 Recursos para Hacer el Robot

- [Lista de materiales detallada](./docs/lista-componentes.md)
- [Guía de armado paso a paso](./docs/guia-armado.md)
- [Archivos STL para impresión 3D](./3d/)

---

## ⚖️ Licencia

Este proyecto está licenciado bajo la [MIT License](./LICENSE).

---

> _¿Quieres contribuir o seguir nuestro avance? Síguenos en nuestras redes oficiales y revisa este repositorio para novedades y recursos._

---

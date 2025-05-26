
---

# Equipo [Team ValGrind] – WRO 2025

<img src="https://github.com/damuec/ValRepo1/assets/logo-equipo.png" alt="Logo del equipo" width="200"/>

---

##  Índice

1. [Nuestro Equipo](#nuestro-equipo)
    - [Integrantes](#integrantes)
    - [Orígenes](#origenes)
2. [Nuestro Objetivo](#nuestro-objetivo)
3. [El Robot](#el-robot)
    - [Apartado Mecánico](#apartado-mecánico)
        - [Movilidad](#movilidad)
        - [Chasis e Impresión 3D](#chasis-e-impresión-3d)
    - [Apartado Electrónico](#apartado-electrónico)
        - [Baterías](#baterías)
        - [Sensores Ultrasónicos](#sensores-ultrasónicos)
        - [Conexiones y Circuitos](#conexiones-y-circuitos)
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

Somos **[Team ValGrind]**, entusiastas de la robótica y la innovación. Representamos a nuestra institución en la fase regional de la **WRO 2025**, con la meta de clasificar a la nacional.

En esta primera sección , se encuentra toda la información básica acerca de nuestro equipo, desde nuestros orígenes hasta nuestros planes para poder lograr todas nuestras metas.
    -[Integrantes]
     Samuel Burgos 
     _png_
     17 años, 09/01/2008
     Un resumen corto


-  [Institución]: [https://www.instagram.com/colegiosaltoangel]
-  [Instagram Oficial]:
(https://instagram.com/team_valgrind)
-  [Contáctanos]: [valgrind2025@gmail.com]

## Nuestros Origenes

El nombre de nuestro equipo fue decidido por los 3 a partir de un clásico compilador de C++ llamado de la misma forma, el cual es representado por un logotipo desértico y un dragón rojo hecho mediante origami. Al mismo tiempo, tanto el apartado estético como la identidad de nuestro equipo también muestra simbolismos y elementos de la mitología nórdica, razón por la cual también decidimos que nuestro robot se llamara "Heimdall", ya que no solo es un guiño a la mitología nórdica en cuestión sino que también cabe destacar que su personaje es un dios en la mitología nórdica, conocido como el guardián de las puertas de Asgard y el puente Bifröst, que conecta los Nueve Mundos. y asimismo También es responsable de anunciar el inicio del Ragnarök. Al ser un personaje imponente, fue elegido como el nombre que representará a nuestro proyecto dentro de la World Robotic Olympiad 2025 (WRO)

<foto del robot>

---

## Nuestro Objetivo

Diseñar, construir y programar un robot autónomo capaz de superar los desafíos de la WRO 2025, utilizando innovación y trabajo en equipo para lograr un desempeño sobresaliente en la etapa regional y avanzar a la nacional. Para llegar a esto, hemos pasado los últimos meses diseñando, construyendo, armando y programando nuestro más nuevo proyecto "Heimdall" 

 La World Robot Olympiad (WRO) es una competencia internacional de robótica educativa que se desarrolla en varias fases, y nuestro equipo lógicamente tiene que comenzar por la fase regional, mediante la cual al ganarla se accede a la Fase Nacional. Cada país organiza su propio torneo clasificatorio, donde equipos de jóvenes compiten en distintas categorías, aplicando conocimientos de robótica y STEAM. Por último, la fase a la cual nuestro equipo quiere llegar es la Final Internacional. Los equipos ganadores de cada país participan en la competencia global, donde enfrentan desafíos más complejos y compiten con representantes de más de 95 países
    - _foto wro_
    - Aún así, nuestro único objetivo no es solamente ganar, como un equipo unido también tenemos la convicción de poder crear un robot no solo funcional, sino que llene nuestras expectativas; A pesar de ser intangible nuestro deseo de poder culminar nuestro proyecto como un equipo unido y verlo brillar, también es una meta importante para nosotros.

---

## El Robot

### Apartado Mecánico

#### Movilidad

Nuestro robot emplea un sistema de tracción diferencial, ofreciendo maniobrabilidad precisa para enfrentar los retos del campo de competencia. El sistema de cruces se realiza mediante un servo que ajusta la dirección del robot en intersecciones críticas.

#### Chasis e Impresión 3D

Fusion 360 es una plataforma de software de modelado 3D, CAD, CAM, CAE y PCB basada en la nube destinada al diseño y la fabricación de productos. Nos permite diseñar y crear productos de acuerdo a sus especificaciones particulares, garantizando que cada pieza cumpla con los más exigentes criterios de estética, forma, ajuste y funcionalidad. Además, incorpora un conjunto integral de herramientas avanzadas para el diseño de placas de circuito impreso y componentes electrónicos, lo que facilita la conceptualización y materialización de cualquier idea, incluso las más complejas. Al implementar estas funciones, la plataforma optimiza significativamente tanto el tiempo como los costos asociados, asegurando que el proceso de producción se realice de manera eficiente y que las piezas obtenidas sean de la más alta calidad. Nosotros usamos esta herramienta gráfica y de diseño para poder realizar los diseños del chasis y aquellas piezas personalizadas para nuestro robot

![Chasis 3D](https://github.com/damuec/ValRepo1/assets/chasis-3d.png)

---

### Apartado Electrónico

#### Baterías

Para el proyecto, decidimos usar dos baterías:

- La primera, se trata de un paquete de 2 baterías recargables de 12 V con una capacidad nominal de 2000 mAh cada una. Utilizan tecnología de níquel-metal hidruro (NiMH), lo que elimina el efecto memoria y garantiza que, a pesar de repetidos ciclos de carga y descarga, la capacidad de energía se conserve de forma óptima. Con dimensiones aproximadas de 50 x 29 x 72 mm y cableado con cables desnudos, la batería permite una integración versátil y directa, lo cual nos facilita integrarlo dentro de nuestro proyecto sin que inmiscuya con el rendimiento de otros componentes, o con el diseño del mismo

<foto>

- La segunda, se trata un kit de baterías recargables Tenergy de tecnología NiMH, diseñado para ofrecer una salida estable de 12 V y una capacidad de 2000 mAh por unidad en un formato compacto y robusto, que facilita su integración en proyectos de electrónica y robótica gracias a sus cables desnudos para conexiones directas; garantizando una carga rápida y un suministro energético continuo y fiable, este kit, a nuestro parecer resulta ideal para aplicaciones exigentes como las competencias de robótica en nuestra categoría , donde es imperativo optimizar tanto el rendimiento del sistema como los tiempos de montaje y costos operativos. 

<fotos>

#### Sensores Ultrasónicos

El robot cuenta con múltiples sensores ultrasónicos (HC-SR04 para ser específicos) ubicados estratégicamente para la detección de obstáculos y el cálculo de distancias, permitiendo navegación autónoma y segura. El sensor en cuestión, es un sensor de distancia de bajo costo, por lo que su uso es muy frecuente en la robótica. Este incorpora un par de transductores de ultrasonido que se utilizan de manera conjunta para determinar la distancia del sensor con un objeto colocado enfrente de este. Un transductor emite una “ráfaga” de ultrasonido y el otro capta el rebote de dicha onda.

El tiempo que tarda la onda sonora en ir y regresar a un objeto puede utilizarse para conocer la distancia que existe entre el origen del sonido y el objeto. La interfaz del sensor HC-SR04 y Arduino se logra mediante 2 pines digitales: el pin de disparo (trigger) y eco (echo). La función de cada uno de estos pines es la siguiente:

El pin trigger recibe un pulso de habilitación de parte del microcontrolador, mediante el cual se le indica al módulo que comience a realizar la medición de distancia.
En el pin echo el sensor devuelve al microcontrolador un puso cuyo ancho es proporcional al tiempo que tarda el sonido en viajar del transductor al obstáculo y luego de vuelta al módulo.

#### Conexiones y Circuitos

Todos los módulos están conectados en un circuito organizado, minimizando interferencias y facilitando el mantenimiento.  

[Ver diagrama del circuito](./docs/diagrama-electronico.png)


#### Microcontroladores

esp-32.

---

### Apartado Programático

/*
  Código de Heimdall para la categoría futuros ingenieros de la WRO 2025
  Hecho por Cristobal Mogollon

  El código hace que el robot al prenderse avance y de 3 vueltas a la pista
  de futuros ingenieros en la etapa abierta y al completar la cantidad de 12
  giros a la pista avanzará dos segundos más y se detiene automáticamente

  Mayo 21 2025
  5:18 PM

*/

// Incluyendo librerías necesarias
```#include <Wire.h>        // Librería para I2C
#include <Ultrasonic.h>
#include <Servo.h>
#include <Adafruit_BNO08x.h> // Librería para el BNO080/BNO085

// Definición de pines para los sensores ultrasónicos
#define USTFRONT 23
#define USEFRONT 22
#define USTRIGHT 45
#define USERIGHT 44
#define USTLEFT 31
#define USELEFT 30

// Creación de objetos Ultrasonic para cada sensor
Ultrasonic USFront(USTFRONT, USEFRONT);
Ultrasonic USLeft(USTLEFT, USELEFT);
Ultrasonic USRight(USTRIGHT, USERIGHT);

const int DISTANCIA_OBSTACULO_FRONTAL = 15; // Distancia en cm para el sensor frontal
const int DISTANCIA_OBSTACULO_LATERAL = 50; // Distancia en cm para los sensores laterales

Servo esc;
Servo myservo;

int pos = 85;
bool motorEnMarcha = false;
int contadorVueltas = 0;         // Contador de vueltas
bool giroDetectado = false;       // Para evitar contar el mismo giro varias veces
bool robotDetenido = false;       // Variable para detener el robot después de 12 giros

// BNO085
Adafruit_BNO08x bno08x;
#define BNO08X_RESET -1           // Si no usas el pin de reset
sh2_SensorValue sensorValue;

void setup() {
  Wire.begin();                    // Inicializa la comunicación I2C para el BNO085 en Arduino Mega
  esc.attach(9, 1000, 2000);
  myservo.attach(11);
  Serial.begin(115200);

  // Inicializar el BNO085
  if (!bno08x.begin_I2C()) {
    Serial.println("¡No se pudo iniciar el BNO08x!");
    while (1) delay(1);
  }
  bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED, 10000); // Reporte cada 10ms

  // Armar el ESC al iniciar
  esc.write(90);
  delay(3000);
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

  // Leer datos del giroscopio del BNO085
  float gyroZ = 0.0;
  if (bno08x.getSensorEvent(&sensorValue)) {
    if (sensorValue.sensorId == SH2_GYROSCOPE_CALIBRATED) {
      gyroZ = sensorValue.un.gyroscope.z * 57.3; // Convertir a dps
    }
  }

  Serial.print("Frontal: "); Serial.print(frontal);
  Serial.print("cm | Izq: "); Serial.print(izquierda);
  Serial.print("cm | Der: "); Serial.print(derecha);
  Serial.print("cm | GyroZ: "); Serial.println(gyroZ);

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

  // Calcular el número de vueltas (simplificado)
  // Usamos una variable giroDetectado para evitar contar el mismo giro varias veces
  if (abs(gyroZ) > 50 && !giroDetectado) { // Ajustar este valor según la sensibilidad del giroscopio
    giroDetectado = true;                   // Marcamos que se ha detectado un giro
    if (gyroZ > 0) {                        // Giro en sentido horario (ejemplo)
      contadorVueltas++;                     // Incrementamos el contador de vueltas
      Serial.print("Vueltas: ");
      Serial.println(contadorVueltas);
    }
  } else if (abs(gyroZ) < 20) {             // Si la velocidad angular es baja, reseteamos giroDetectado
    giroDetectado = false;
  }

  // Si contadorVueltas llega a 12, avanzar 2 segundos y parar
  if (contadorVueltas >= 12) {
    Adelante(); // Avanzamos
    delay(2000);  // Esperamos 2 segundos
    Parar();    // Detenemos el motor
    Serial.println("¡Completadas 12 vueltas!");
    robotDetenido = true;  // Bloquea para que no siga ejecutando
  }
}

void Adelante(){
  Serial.println("Armando ESC...");
  esc.write(90);     // Pulso mínimo para armar ESC
  delay(10);                     // Espera breve para armar

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
```

#### Diagramas de Flujo

![Diagrama de Flujo](./docs/diagrama-flujo.png)


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

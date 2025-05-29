
---

# Equipo [Team ValGrind]: WRO 2025

[![SAVE-20250527-193558.jpg](https://i.postimg.cc/VvXNFbns/SAVE-20250527-193558.jpg)](https://postimg.cc/fSTZzy81)

---

## Índice

1. [Nuestro Equipo](#nuestro-equipo)
    - [Integrantes](#integrantes)
    - [Origenes](#origenes)
    - [Nuestro Objetivo](#nuestro-objetivo)
3. [El Robot](#el-robot)
    - [Apartado Mecánico](#apartado-mecánico)
        - [Diseño 3D](#diseño-3d)
        - [Movilidad](#movilidad)
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
  - [Mecánica](#mecánica)
  - [Electrónica](#electrónica)
  - [Programación](#programación)
5. [Licencia](#licencia)

---

## Nuestro Equipo

<img src="https://github.com/damuec/ValRepo1/assets/foto-equipo.jpg" alt="Foto del equipo" width="400"/>

Somos **[Team ValGrind]**, entusiastas de la robótica y la innovación. Representamos a nuestra institución en la fase regional de la **W nuestras metas.

---

### Integrantes

- **Samuel Burgos**  
  17 años, 09/01/2008  
  Un resumen corto

- **Sebastián Salina**  
  17 años, 09/01/2008  
  Un resumen corto

- **Cristobal Mogoll  17 años, 09/01/2008  
  Un resumen corto

- **Institución:** [Colegio Salto Ángel](https://www.instagram.com/colegiosaltoangel)
- **Instagram Oficial:** [@team_valgrind](https://instagram.com/team_valgrind)
- **Contáctanos:** [valgrind2025@gmail.com](mailto:valgrind2025@gmail.com)

---

### Origenes

El nombre de nuestro equipo fue decidido por los tres integrantes a partir de un clásico compilador de C++ llamado de la misma forma, el cual es representado por un logotipo des". Este nombre no solo es un guiño a la mitología mencionada, sino que además corresponde al dios conocido como el guardián de las puertas de Asgard y del puente Bifröst, que conecta los Nueve Mundos. Al ser un personaje imponente, consideramos que es un nombre apropiado para nuestroHeimdall".

La World Robot Olympiad (WRO) es una competencia internacional de robótica educativa que se desarrolla en varias fases, y nuestro equipo debe comenzar por la fase regional, mediante la cual, al ganarla, se accede a la Fase Nacional. Cada país organiza su propio torneo clasificatorio, donde equipos de jóvenes compiten en distintas categorías, aplicando conocimientos de robótica y STEAM. Por último, la fase a la que nuestro equipo quiere llegar es la Final Internacional, donde los equipos ganadores de cada país participan en la competencia global, enfrentando desafíos más complejos y compitiendo con representantes de más de 95 países.

- _foto wro_

Aun así, nuestro único objetivo no es solamente ganar. Como equipo unido, también tenemos la convicción de crear un robot no solo funcional, sino que cumpla nuestras expectativas. A pesar de ser intangible, nuestro deseo de culminar nuestro proyecto como un equipo unido y verlo brillar también es una meta importante para nosotros.

## Nuestro Objetivo

Diseñar, construir y programar un robot autónomo capaz de superar los desafíos de la WRO 2025, utilizando innovación y trabajo en equipo para lograr un desempeño sobresaliente en la etapa regional y avanzar a la nacional. Para llegar a esto, hemos pasado los últimos meses diseñando, construyendo, armando y programando nuestro más nuevo proyecto "Heimdall"

La World Robot Olympiad (WRO) es una competencia internacional de robótica educativa que se desarrolla en varias fases, y nuestro equipo lógicamente tiene que comenzar por la fase regional, mediante la cual al ganarla se accede a la Fase Nacional. Cada país organiza su propio torneo clasificatorio, donde equipos de jóvenes compiten en distintas categorías, aplicando conocimientos de robótica y STEAM. Por último, la fase a la cual nuestro equipo quiere llegar es la Final Internacional. Los equipos ganadores de cada país participan en la competencia global, donde enfrentan desafíos más complejos y compiten con representantes de más de 95 países - foto wro - Aún así, nuestro único objetivo no es solamente ganar, como un equipo unido también tenemos la convicción de poder crear un robot no solo funcional, sino que llene nuestras expectativas; A pesar de ser intangible nuestro deseo de poder culminar nuestro proyecto como un equipo unido y verlo brillar, también es una meta importante para nosotros.

---

## El Robot

### Apartado Mecánico

#### Diseño 3D

Fusion 360 es una plataforma de software de modelado 3D, CAD, CAM, CAE y PCB basada en la nube, destinada al diseño y la fabricación de productos. Nos permite diseñar y crear productos de acuerdo a sus especificaciones particulares, garantizando que cada pieza cumpla con los más exigentes criterios de estética, forma, ajuste y funcionalidad. Además, incorpora un conjunto integral de herramientas avanzadas para el diseño de placas de circuito impreso y componentes electrónicos, lo que facilita la conceptualización y materialización de cualquier idea, incluso las más complejas. Al implementar estas funciones, la plataforma optimiza significativamente tanto el tiempo como los costos asociados, asegurando que el proceso de producción se realice de manera eficiente y que las piezas obtenidas sean de la más alta calidad. Nosotros usamos esta herramienta gráfica y de diseño para realizar los diseños del chasis y las piezas personalizadas de nuestro robot.

![Chasis 3D](https://github.com/damuec/ValRepo1/assets/chasis-3d.png)

#### Movilidad

Nuestro robot emplea un sistema de tracción diferencial, ofreciendo maniobrabilidad precisa para enfrentar los retos del campo de competencia. El sistema de cruces se realiza mediante un servo que ajusta la dirección del robot en intersecciones críticas.

---

### Apartado Electrónico

#### Baterías

Para el proyecto, decidimos usar dos baterías:

- La primera es un paquete de 2 baterías recargables de 12 V con una capacidad nominal de 2000 mAh cada una. Utilizan tecnología de níquel-metal hidruro (NiMH), lo que elimina el efecto memoria y garantiza que, a pesar de repetidos ciclos de carga y descarga, la capacidad de energía se conserve de forma óptima. Con dimensiones aproximadas de 50 x 29 x 72 mm y cableado con cables desnudos, la batería permite una integración versátil y directa, lo cual facilita su integración dentro de nuestro proyecto sin afectar el rendimiento de otros componentes o el diseño.

<foto>

- La segunda es un kit de baterías recargables Tenergy, también de tecnología NiMH, diseñado para ofrecer una salida estable de 12 V y una capacidad de 2000 mAh por unidad en un formato compacto y robusto, que facilita su integración en proyectos de electrónica y robótica gracias a sus cables desnudos para conexiones directas. Garantizando una carga rápida y un suministro energético continuo y fiable, este kit nos resulta ideal para aplicaciones exigentes como las competencias de robótica en nuestra categoría, donde es imperativo optimizar tanto el rendimiento del sistema como los tiempos de montaje y costos operativos.

<fotos>

#### Sensores Ultrasónicos

El robot cuenta con múltiples sensores ultrasónicos (HC-SR04) ubicados estratégicamente para la detección de obstáculos y el cálculo de distancias, permitiendo navegación autónoma y segura. El sensor HC-SR04 es un sensor de distancia de bajo costo, por lo que su uso es muy frecuente en la robótica. Incorpora un par de transductores de ultrasonido que se utilizan de manera conjunta para determinar la distancia del sensor con un objeto colocado frente a este. Un transductor emite una ráfaga de ultrasonido y el otro capta el rebote de dicha onda.

El tiempo que tarda la onda sonora en ir y regresar a un objeto puede utilizarse para conocer la distancia entre el origen del sonido y el objeto. La interfaz del sensor HC-SR04 y Arduino se logra mediante 2 pines digitales: el pin de disparo (trigger) y el pin de eco (echo). La función de cada uno de estos pines es la siguiente:

- El pin trigger recibe un pulso de habilitación del microcontrolador, mediante el cual se le indica al módulo que comience a realizar la medición de distancia.
- En el pin echo el sensor devuelve al microcontrolador un pulso cuyo ancho es proporcional al tiempo que tarda el sonido en viajar del transductor al obstáculo y luego de vuelta al módulo.

#### Conexiones y Circuitos

Todos los módulos están conectados en un circuito organizado, minimizando interferencias y facilitando el mantenimiento.  

[Ver diagrama del circuito](./docs/diagrama-electronico.png)

#### Microcontroladores

Utilizamos ESP32.

---

### Apartado Programático

#### Código por Componente

En cuanto al código utilizado para manejar el robot, consiste en una parte en la que se definen los pines del BNO085, del ESC y de los ultrasónicos. Dentro del código se arma el ESC, se inicializan los sensores y se inicializa una función llamada "doceVueltas", la cual se encarga de hacer una lectura constante de los sensores ultrasónicos para decidir en qué momento girar, así como de registrar los giros para que el robot se detenga al completar exitosamente 3 vueltas.

En este apartado se inicializan los pines:
```cpp
void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);  // Inicializar I2C con pines específicos
  esc.attach(PIN_ESC, 1000, 2000);
  myservo.attach(PIN_SERVO);
  Serial.begin(115200);

  // Inicializar BNO085
  if (!bno08x.begin_I2C(0x4B, &Wire)) {
    Serial.println("¡No se pudo iniciar el BNO08x!");
    while(1);
  }
  bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED, 10000);

  // Armar el ESC al iniciar
  esc.write(90);
  delay(3000);

  tiempoAnterior = millis();
}
```

En este apartado, se llama a la función `doceVueltas`, se hace una lectura de los sensores y se calibra el giroscopio:

```cpp
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
}
```

Dentro de `Open.ino` está el resto de funciones descritas, y la lógica de programación mediante la cual el robot completa el desafío abierto.

#### Diagramas de Flujo

En este diagrama de flujo se halla una representación gráfica del funcionamiento lógico de nuestra programación, así como de lo que se espera sea el desempeño del robot al inicializar el programa.

[![IMG-20250523-WA0008.jpg](https://i.postimg.cc/QxYhNwBT/IMG-20250523-WA0008.jpg)](https://postimg.cc/YhFJbXzr)

#### Algoritmos y Teoría

_(Se sugiere añadir aquí una breve explicación de los algoritmos principales y la teoría usada, si aplica)_

#### Compiladores y Comunicación

- **Lenguaje principal:** C++ (Arduino IDE)
- **Compilador:** [Arduino IDE](https://www.arduino.cc/en/software)
- **Comunicación entre módulos:** Bus I2C y SPI

---

## Recursos para Hacer el Robot

- [Lista de materiales detallada](./docs/lista-componentes.md)
- [Guía de armado paso a paso](./docs/guia-armado.md)
- [Archivos STL para impresión 3D](./3d/)

---

## Licencia

_(Incluye aquí el tipo de licencia, por ejemplo MIT, GPL, Creative Commons, etc. Si tienes un archivo LICENSE, referencia o resume aquí el contenido principal)_

---

> _¿Quieres contribuir o seguir nuestro avance? Síguenos en nuestras redes oficiales y revisa este repositorio para novedades y recursos._

---


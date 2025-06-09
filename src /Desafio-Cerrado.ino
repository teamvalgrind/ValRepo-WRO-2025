void Derecha() {
  esc.write(130);
  myservo.write(30);      // Giro máximo
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
  myservo.write(50);      // Giro leve (ajusta el ángulo a tu servo)
  unsigned long inicio = millis();
  while (millis() - inicio < (DURACION_GIRO_MS/2)) { // Duración menor
    delay(10);
  }
  myservo.write(99);
  esc.write(90);
  Serial.println("Giro leve derecha para esquivar bloque");
  Adelante();
}

void Izquierda() {
  esc.write(130);
  myservo.write(150);     // Giro máximo
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
  myservo.write(130);     // Giro leve (ajusta el ángulo a tu servo)
  unsigned long inicio = millis();
  while (millis() - inicio < (DURACION_GIRO_MS/2)) { // Duración menor
    delay(10);
  }
  myservo.write(99);
  esc.write(90);
  Serial.println("Giro leve izquierda para esquivar bloque");
  Adelante();
}
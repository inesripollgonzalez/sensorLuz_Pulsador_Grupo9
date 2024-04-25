#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

#define PULSADOR_PIN 14 //Antes5
#define LED_VERDE_PIN 0
//Sensor luz conectado a D1 (SCL) y D2 (SDA)

int contadorPulsos = 0;
int brillo = 0;
int modo = 0;
bool pulsadorPresionado = false;
float luz = 0;
int begin = 0;

void setup() {
  pinMode(PULSADOR_PIN, INPUT_PULLUP); //INPUT_PULLUP
  pinMode(LED_VERDE_PIN, OUTPUT);

  apagarLeds();

  Serial.begin(9600);

  Wire.begin();

  lightMeter.begin();
  Serial.println(F("BH1750 Test"));
}

void loop() {
  // Leer la entrada del usuario desde la terminal serial
  if (Serial.available() > 0) {
    Serial.println("Selecciona un modo: \nA - Pulsador\nB - Sensor Luz \n");
    char entrada = Serial.read(); // Leer el carácter ingresado por el usuario
    if (entrada == 'A' || entrada == 'a') {
      modo = 1;
      apagarLeds();
      Serial.println("Modo seleccionado: Pulsador\n");
    } else if (entrada == 'B' || entrada == 'b') {
      modo = 2;
      apagarLeds();
      Serial.println("Modo seleccionado: Sensor de Luz\n");
    }
  }
  
  if (modo == 1) {
    modoPulsador();
  } else if (modo == 2) {
    modoSensorLuz();
  }
}

void modoSensorLuz() {
    //Convertir la lectura de luz en brillo para el LED (0-255)
    luz = lightMeter.readLightLevel();
    // El maximo del sensor es 65535, pero par que se note mas el cambio, el limite superior se ha bajado.
    brillo = map(luz, 0, 30000, 255, 0); 
    analogWrite(LED_VERDE_PIN, brillo);
}

void modoPulsador () {
  if (digitalRead(PULSADOR_PIN) == LOW && pulsadorPresionado == false) {
    // Esperar un breve tiempo para evitar falsos positivos
    delay(50);
    if (digitalRead(PULSADOR_PIN) == LOW) {
      // Incrementar el contador de pulsos
      contadorPulsos++;
      if (contadorPulsos == 4) {
        contadorPulsos = 0;
      }
      pulsadorPresionado = true;
    }
  } else if (digitalRead(PULSADOR_PIN) == HIGH) {
    pulsadorPresionado = false;
  }

  brillo = 0;
  switch (contadorPulsos) {
    case 0:
      brillo = 0; // Apagar el LED
      break;
    case 1:
      brillo = 50; // Baja intensidad
      break;
    case 2:
      brillo = 100; // Media intensidad
      break;
    case 3:
      brillo = 255; // Alta intensidad
      break;
  }
  analogWrite(LED_VERDE_PIN, brillo);
}

void apagarLeds () {
  digitalWrite(LED_VERDE_PIN, LOW);
}

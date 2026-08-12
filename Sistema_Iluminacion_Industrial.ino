#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>

// =====================================
// PINES
// =====================================

#define PIR_PIN 2
#define DHT_PIN 3
#define LED_PIN 5

#define DHTTYPE DHT11

// =====================================
// SENSORES
// =====================================

BH1750 sensorLuz;
DHT dht(DHT_PIN, DHTTYPE);

// =====================================
// CONFIGURACIÓN DE ILUMINACIÓN
// =====================================

// A partir de este nivel consideramos
// que ya existe suficiente iluminación.
const float LUX_MAX = 50.0;

// Potencia estimada de la iluminación
// utilizada para calcular consumo.
const float POTENCIA_LED = 10.0;

// =====================================
// VARIABLES
// =====================================

float lux = 0;
float temperatura = 0;
float humedad = 0;

int presencia = 0;
int intensidadPWM = 0;
int porcentajeLED = 0;

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Wire.begin();

  // Iniciar BH1750
  if (sensorLuz.begin()) {
    Serial.println("BH1750: OK");
  } 
  else {
    Serial.println("BH1750: ERROR");
  }

  // Iniciar DHT11
  dht.begin();

  // LED inicialmente apagado
  analogWrite(LED_PIN, 0);

  Serial.println("--------------------------------------");
  Serial.println("SISTEMA DE ILUMINACION INDUSTRIAL");
  Serial.println("MODO: AUTOMATICO");
  Serial.println("--------------------------------------");

}

// =====================================
// LOOP
// =====================================

void loop() {

  // ===================================
  // LECTURA DE SENSORES
  // ===================================

  lux = sensorLuz.readLightLevel();

  presencia = digitalRead(PIR_PIN);

  temperatura = dht.readTemperature();

  humedad = dht.readHumidity();


  // ===================================
  // CONTROL AUTOMÁTICO DE ILUMINACIÓN
  // ===================================

  intensidadPWM = 0;

  // Si NO hay persona:
  // iluminación apagada.
  if (presencia == LOW) {

    intensidadPWM = 0;

  }

  // Si HAY persona:
  else {

    // Muy poca iluminación
    if (lux <= 20) {

      intensidadPWM = 255;
    }

    // Poca iluminación
    else if (lux <= 30) {

      intensidadPWM = 191;
    }

    // Iluminación media
    else if (lux <= 40) {

      intensidadPWM = 128;
    }

    // Iluminación suficiente pero baja
    else if (lux <= 50) {

      intensidadPWM = 64;
    }

    // Mucha iluminación
    else {

      intensidadPWM = 0;
    }

  }


  // ===================================
  // APLICAR INTENSIDAD AL LED
  // ===================================

  analogWrite(LED_PIN, intensidadPWM);


  // ===================================
  // CONVERTIR PWM A PORCENTAJE
  // ===================================

  porcentajeLED = map(intensidadPWM, 0, 255, 0, 100);


  // ===================================
  // MOSTRAR DATOS
  // ===================================

  Serial.print("LUX=");
  Serial.print(lux, 2);

  Serial.print(",PRESENCIA=");

  if (presencia == HIGH) {
    Serial.print("SI");
  }
  else {
    Serial.print("NO");
  }

  Serial.print(",TEMPERATURA=");

  if (isnan(temperatura)) {
    Serial.print("ERROR");
  }
  else {
    Serial.print(temperatura, 1);
  }

  Serial.print(",HUMEDAD=");

  if (isnan(humedad)) {
    Serial.print("ERROR");
  }
  else {
    Serial.print(humedad, 1);
  }

  Serial.print(",LED=");
  Serial.print(porcentajeLED);

  Serial.println("%");


  // ===================================
  // ESPERA
  // ===================================

  delay(2000);

}

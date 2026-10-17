#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_PCF8574.h>
#include "SparkFunBMP384.h"
#include <math.h>
// =========================
// Φίλιπ Γρηγοριάδης
//
// =========================
// DHT11
// =========================
#define DHTPIN A2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// =========================
// BMP384
// =========================
BMP384 pressureSensor;

// =========================
// LCD
// =========================
LiquidCrystal_PCF8574 lcd(0x27);

// =========================
// ΡΥΘΜΙΣΕΙΣ
// =========================

// Εδώ είναι το ph0 του Θησείου
const float SEA_LEVEL_PRESSURE_HPA = 1020.8;

// Σταθερές βαρομετρικής εξίσωσης
const float R = 8.314462618;
const float M = 0.0289644;
const float g = 9.80665;

// Μετρητής δειγμάτων
unsigned long sampleNumber = 0;


void setup() {

  Serial.begin(115200);

  // =========================
  // LCD
  // =========================
  Wire.begin();

  lcd.begin(16, 2);
  lcd.setBacklight(255);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Altimeter Init");
  delay(1000);

  // =========================
  // DHT11
  // =========================
  dht.begin();

  // =========================
  // BMP384
  // =========================
  Wire1.begin();

  if (pressureSensor.beginI2C(0x77, Wire1) != BMP3_OK) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BMP384 ERROR");

    Serial.println("BMP384 Init Failed!");

    while (1);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");

  delay(2000);

  // =========================
  // CSV HEADER
  // =========================

  Serial.println("sample,temperature_C,pressure_hPa,p0_hPa,barometric_altitude_m");
}


void loop() {

  // =========================
  // ΘΕΡΜΟΚΡΑΣΙΑ
  // =========================

  float temperatureC = dht.readTemperature();

  if (isnan(temperatureC)) {

    Serial.println("DHT11_ERROR");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT11 ERROR");

    delay(2000);
    return;
  }

  // Celsius -> Kelvin
  float temperatureK = temperatureC + 273.15;


  // =========================
  // ΠΙΕΣΗ
  // =========================

  bmp3_data sensorData;

  if (pressureSensor.getSensorData(&sensorData) != BMP3_OK) {

    Serial.println("BMP384_ERROR");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BMP384 ERROR");

    delay(1000);
    return;
  }

  // Pa -> hPa
  float pressureHPa = sensorData.pressure / 100.0;


  // =========================
  // ΒΑΡΟΜΕΤΡΙΚΗ ΕΞΙΣΩΣΗ
  // =========================

  float altitude =
      (R * temperatureK) / (M * g)
      * log(SEA_LEVEL_PRESSURE_HPA / pressureHPa);


  // =========================
  // CSV OUTPUT
  // =========================

  Serial.print(sampleNumber);
  Serial.print(",");

  Serial.print(temperatureC, 2);
  Serial.print(",");

  Serial.print(pressureHPa, 2);
  Serial.print(",");

  Serial.print(SEA_LEVEL_PRESSURE_HPA, 2);
  Serial.print(",");

  Serial.println(altitude, 2);


  // =========================
  // LCD
  // =========================

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(altitude, 1);
  lcd.print("m T:");
  lcd.print(temperatureC, 1);

  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(pressureHPa, 1);
  lcd.print("hPa");


  sampleNumber++;

  // Νέα μέτρηση κάθε 2 sec
  delay(2000);
}
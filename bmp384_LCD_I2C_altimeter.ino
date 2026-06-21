#include <Wire.h>
#include "SparkFunBMP384.h"
#include <LiquidCrystal_PCF8574.h> // Συμβατή βιβλιοθήκη για UNO R4

BMP384 pressureSensor;

// Αρχικοποίηση οθόνης LCD στη διεύθυνση 0x27
LiquidCrystal_PCF8574 lcd(0x27); 

// -----------------------------------------------------------------
// Τυπική ατμοσφαιρική πίεση στη θάλασσα σε Hectopascals (hPa).
// Η παγκόσμια μέση τιμή είναι 1013.25 hPa.
// Αν το υψόμετρο έχει απόκλιση, αλλάξτε αυτή την τιμή με τον τρέχοντα καιρό.
// -----------------------------------------------------------------
const float SEA_LEVEL_PRESSURE_HPA = 1013.25; 

void setup() {
  Serial.begin(115200);
  
  // Αρχικοποίηση της οθόνης LCD (Default Wire στα pins A4/A5)
  Wire.begin();
  lcd.begin(16, 2);
  lcd.setBacklight(255); // Ανάβει τον οπίσθιο φωτισμό
  
  lcd.setCursor(0, 0);
  lcd.print("Altimeter Init...");

  // Αρχικοποίηση της θύρας Qwiic (Wire1) για τον αισθητήρα
  Wire1.begin(); 

  if (pressureSensor.beginI2C(0x77, Wire1) != BMP3_OK) {
    lcd.clear();
    lcd.print("Sensor Error!");
    Serial.println("BMP384 Init Failed!");
    while (1);
  }

  lcd.clear();
  Serial.println("System Ready!");
}

void loop() {
  bmp3_data sensorData;

  // Λήψη δεδομένων από τον αισθητήρα
  if (pressureSensor.getSensorData(&sensorData) == BMP3_OK) {
    
    // 1. Μετατροπή της πίεσης από Pascal σε hPa
    float currentPressureHPa = sensorData.pressure / 100.0; 

    // 2. Μαθηματικός Υπολογισμός Υψομέτρου (Διεθνής Βαρομετρική Φόρμουλα)
    float altitude = 44330.0 * (1.0 - pow((currentPressureHPa / SEA_LEVEL_PRESSURE_HPA), 0.1903));

    // --- Εμφάνιση στην οθόνη LCD ---
    // 1η Γραμμή: Υψόμετρο
    lcd.setCursor(0, 0);
    lcd.print("Altitude: ");
    lcd.print(altitude, 1); // 1 δεκαδικό ψηφίο
    lcd.print(" m   ");     // Τα κενά σβήνουν προηγούμενα ψηφία

    // 2η Γραμμή: Πίεση σε hPa
    lcd.setCursor(0, 1);
    lcd.print("Pres: ");
    lcd.print(currentPressureHPa, 1);
    lcd.print(" hPa   ");

    // Εκτύπωση στη σειριακή θύρα για έλεγχο
    Serial.print("Pressure: "); Serial.print(currentPressureHPa);
    Serial.print(" hPa | Altitude: "); Serial.print(altitude); Serial.println(" m");

  } else {
    lcd.setCursor(0, 0);
    lcd.print("Read Error!     ");
  }

  delay(1000); // Ανανέωση ανά 1 δευτερόλεπτο
}

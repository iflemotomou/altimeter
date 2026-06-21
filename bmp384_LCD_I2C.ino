#include <Wire.h>
#include "SparkFunBMP384.h"
#include <LiquidCrystal_I2C.h>

// Δημιουργία αντικειμένου για τον αισθητήρα (Θύρα Qwiic - Wire1)
BMP384 pressureSensor;

// Δημιουργία αντικειμένου για την οθόνη LCD (Pins A4/A5 - Wire)
// Συνήθης διεύθυνση I2C για οθόνες LCD είναι η 0x27 (ή 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // Αρχικοποίηση Σειριακής
  Serial.begin(115200);
  
  // Αρχικοποίηση οθόνης LCD (χρησιμοποιεί το default Wire)
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Init System...");

  // Αρχικοποίηση της θύρας Qwiic για τον αισθητήρα
  Wire1.begin(); 

  // Έναρξη αισθητήρα στη διεύθυνση 0x77 μέσω Wire1
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
    
    float temp = sensorData.temperature;
    float press = sensorData.pressure / 100.0; // Μετατροπή σε hPa

    // --- Εμφάνιση στην οθόνη LCD ---
    // 1η Γραμμή: Θερμοκρασία
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp, 1); // 1 δεκαδικό ψηφίο
    lcd.print((char)223); // Χαρακτήρας για το σύμβολο των βαθμών (°)
    lcd.print("C   ");   // Κενά για να σβήνουν παλιά ψηφία

    // 2η Γραμμή: Πίεση
    lcd.setCursor(0, 1);
    lcd.print("Pres: ");
    lcd.print(press, 1);
    lcd.print(" hPa   ");

    // Ταυτόχρονη εκτύπωση στη σειριακή για έλεγχο
    Serial.print("T: "); Serial.print(temp);
    Serial.print(" C | P: "); Serial.print(press); Serial.println(" hPa");

  } else {
    lcd.setCursor(0, 0);
    lcd.print("Read Error!     ");
  }

  delay(1000); // Ανανέωση κάθε 1 δευτερόλεπτο
}

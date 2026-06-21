#include <Wire.h>
#include "SparkFunBMP384.h" // Εισαγωγή της επίσημης βιβλιοθήκης της SparkFun

// Δημιουργία αντικειμένου για τον αισθητήρα
BMP384 pressureSensor;

void setup() {
  // Αρχινοποίηση της σειριακής θύρας
  Serial.begin(115200);
  while (!Serial); // Αναμονή μέχρι να ανοίξει το Serial Monitor
  Serial.println("--- BMP384 Qwiic Test on UNO R4 WiFi ---");

  // Αρχικοποίηση της γραμμής I2C που αντιστοιχεί στη θύρα Qwiic
  Wire1.begin(); 

  // Αρχικοποίηση του αισθητήρα με τη διεύθυνση 0x77 και το Wire1
  if (pressureSensor.beginI2C(0x77, Wire1) != BMP3_OK) {
    Serial.println("Error: Ο αισθητήρας BMP384 δεν εντοπίστηκε! Ελέγξτε τις συνδέσεις.");
    while (1); // "Πάγωμα" του προγράμματος αν αποτύχει η σύνδεση
  }

  Serial.println("Ο αισθητήρας BMP384 συνδέθηκε επιτυχώς!");
}

void loop() {
  // Δημιουργία της ειδικής δομής δεδομένων που απαιτεί η βιβλιοθήκη
  bmp3_data sensorData;

  // Ανάγνωση δεδομένων από τον αισθητήρα
  int8_t err = pressureSensor.getSensorData(&sensorData);

  // Έλεγχος αν η ανάγνωση ήταν επιτυχής (BMP3_OK)
  if (err == BMP3_OK) {
    
    // Εμφάνιση Θερμοκρασίας (Κελσίου) από τη δομή δεδομένων
    Serial.print("Temperature: ");
    Serial.print(sensorData.temperature, 2);
    Serial.print(" °C");

    // Εμφάνιση Πίεσης (μετατροπή από Pascal σε hPa διαιρώντας με το 100)
    Serial.print("  |  Pressure: ");
    Serial.print(sensorData.pressure / 100.0, 2);
    Serial.println(" hPa");
    
  } else {
    Serial.print("Αποτυχία λήψης δεδομένων. Κωδικός σφάλματος: ");
    Serial.println(err);
  }

  delay(2000); // Ανάγνωση και ανανέωση κάθε 1 δευτερόλεπτο
}

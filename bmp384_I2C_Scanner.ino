#include <Wire.h>

void setup() {
  // Αρχικοποίηση της γραμμής I2C που αντιστοιχεί στη θύρα Qwiic
  Wire1.begin(); 
  
  Serial.begin(115200);
  while (!Serial); // Αναμονή μέχρι να ανοίξει το Serial Monitor
  
  Serial.println("\n--- Qwiic Port I2C Scanner ---");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning Qwiic Bus...");

  for (address = 1; address < 127; address++) {
    // Χρησιμοποιούμε το Wire1 για τη θύρα Qwiic
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

    if (error == 0) {
      Serial.print("Device found at address: 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      
      // Έλεγχος αν η διεύθυνση αντιστοιχεί στο BMP384 της SparkFun
      if (address == 0x76 || address == 0x77) {
        Serial.print(" (Potential BMP384 Sensor!)");
      }
      Serial.println();
      nDevices++;
    } 
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found on Qwiic port.\n");
  } else {
    Serial.println("Scan finished.\n");
  }

  delay(5000); // Επανάληψη σάρωσης κάθε 5 δευτερόλεπτα
}


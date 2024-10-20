#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN A3  // Reset pin for RFID
#define SS_PIN A5   // Slave select pin for RFID

MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600); // Initialize serial communication
  SPI.begin();        // Initialize SPI bus
  rfid.PCD_Init();    // Initialize RFID reader
  Serial.println("RFID Reader Initialized");
}

void loop() {
  checkRFID(); // Continuously check for RFID tags
}

// Function to check for RFID tags for emergency vehicle detection
void checkRFID() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String tagID = "";

    // Read the UID of the RFID card
    for (byte i = 0; i < rfid.uid.size; i++) {
      tagID += String(rfid.uid.uidByte[i]); // Construct the tag ID as a String
    }

    // Print the read tagID for debugging
    Serial.println("Detected UID: " + tagID); 

    // Check which emergency vehicle tag is detected and send a unique value
    if (tagID == "67284119") {
      Serial.println("Emergency vehicle detected (ID: 67284119)!");
      Serial.write(1); // Send value for tag 67284119
    } else if (tagID == "192201219") {
      Serial.println("Emergency vehicle detected (ID: 192201219)!");
      Serial.write(2); // Send value for tag 192201219
    } else if (tagID == "31631019") {
      Serial.println("Emergency vehicle detected (ID: 31631019)!");
      Serial.write(3); // Send value for tag 31631019
    } else if (tagID == "17924424818") {
      Serial.println("Emergency vehicle detected (ID: 17924424818)!");
      Serial.write(4); // Send value for tag 17924424818
    } else {
      Serial.println("Non-emergency vehicle detected: " + tagID);
    }
    
    rfid.PICC_HaltA(); 
    rfid.PCD_StopCrypto1(); 
  }
}


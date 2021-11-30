#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "uids.h"

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
String uid = String();
Servo servo;

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
	servo.attach(3);
	servo.write(0);
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	for (byte i = 0; i < (&mfrc522.uid)->size; i++) {
		uid += String((&mfrc522.uid)->uidByte[i], HEX);
	}
	if (DEBUG)
		Serial.println(uid);
	if (uid.equals(BEN) || uid.equals(ADI) || uid.equals(IRIS)) {
		servo.write(90);
		Serial.println("Unlocked");
		delay(2000); // Need to increase this in the actual version to give time for the door to close
		servo.write(0);
		Serial.println("Locked");
	} else {
		Serial.println("Invalid card");
	}
	uid = String();
}

#include <Audio.h>
#include <MFRC522.h>
#include "Arduino.h"
#include "SD.h"
#include "FS.h"
#include "Registry.h"
#include "AudioManager.h"

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

#define RFID_SS 15
#define RFID_RST 22

// microSD Card Reader connections
#define SD_CS         32 //5
#define SPI2_MOSI      13 //23
#define SPI2_MISO      12 //19
#define SPI2_SCK       14 //18

#define MS_BETWEEN_FIT_CHECKS 200 // @grace will need to be tuned to whatever seems to work well.


Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);
MFRC522 rfid(RFID_SS, RFID_RST); // Instance of the class

MFRC522::MIFARE_Key key;
byte nuidPICC[4];

SPIClass spiSD(HSPI);

AudioManager audio_manager;
Registry registry;

AudioState nextAudioState = {"", "", ""};

unsigned long msAtLastCheck = 0; // ms elapsed since board startup, to be used in timing stuff (delay is blocking and since we're doing audio stuff we don't want that)
unsigned int timeToWaitBeforeTestFitChecks = 5000;
char uuid1[] = "testUUID1";
char uuid2[] = "testUUID2";
char uuid3[] = "testUUID3";

bool headEnabled, topEnabled, bottomEnabled = false;

void setup()
{
  Serial.begin(115200);
  // Set microSD Card CS as OUTPUT and set HIGH
  pinMode(SD_CS, OUTPUT); // FIXME may be an issue if nico also has to instantiate.
  digitalWrite(SD_CS, HIGH);

  // Initialize SPI bus for microSD Card
  spiSD.begin(SPI2_SCK, SPI2_MISO, SPI2_MOSI);

  // Initialize SPI bus for RFID tag
  SPI.begin();

  // RFID setup
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  rfid.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  // Start Serial Port
  Serial.begin(115200);

  // Start microSD Card
  if (!SD.begin(SD_CS, spiSD))
  {
    Serial.println("Error accessing microSD card!");
    while (true)
      ;
  }
  Serial.println("SD card initialized!");



  registry.initialize();
  Serial.println("Registry initialized successfully!");

  audio_manager.updateTracks(nextAudioState);
  xTaskCreatePinnedToCore(mainBusinessLogic, "TaskCore0", 10000, NULL, 2, &TaskCore0, 1);
  xTaskCreatePinnedToCore(audioHandler, "TaskCore1", 10000, NULL, 1, &TaskCore1, 0);

}

void mainBusinessLogic(void *pvParameters) {
  for (;;) {

    // will need to be edited to check the fit, and if it's different update the tracks.
    //    while (Serial.available()) {
    //      String serialData = Serial.readString();
    //      if (serialData.indexOf("head") > -1) {
    //        Serial.println("Starting head");
    //
    //        nextAudioState.head_track = registry.getPairByRFID(serialData.c_str()).track.c_str();
    //        audio_manager.updateTracks(nextAudioState);
    //      }
    //      if (serialData.indexOf("top") > -1) {
    //        nextAudioState.top_track = registry.getPairByRFID(serialData.c_str()).track.c_str();
    //        Serial.println("Starting Top");
    //        audio_manager.updateTracks(nextAudioState);
    //      }
    //      if (serialData.indexOf("bottom") > -1) {
    //        Serial.println("Starting bottom");
    //
    //        nextAudioState.bottom_track = registry.getPairByRFID(serialData.c_str()).track.c_str();
    //        audio_manager.updateTracks(nextAudioState);
    //      }
    //    }

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! rfid.PICC_IsNewCardPresent()){
      delay(300);
      continue;
    }

    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial()){
      delay(300);
      continue;
    }

    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      Serial.println(F("Your tag is not of type MIFARE Classic."));
      delay(300);
      continue;
    }

    if (true) {

      // Store NUID into nuidPICC array
      for (byte i = 0; i < 4; i++) {
        nuidPICC[i] = rfid.uid.uidByte[i];
      }

      Serial.println(F("The NUID tag is:"));
      Serial.print(F("In hex: "));
      printHex(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
      Serial.print(F("In dec: "));
      printDec(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
      nextAudioState.head_track = registry.getPairByRFID(uuid1).track.c_str();
      audio_manager.updateTracks(nextAudioState);
      headEnabled = true;
      delay(667);

    }



    //    if ((millis() - msAtLastCheck) > timeToWaitBeforeTestFitChecks && !headEnabled) {
    //      Serial.println(registry.getPairByRFID(uuid1).track.c_str());
    //      nextAudioState.head_track = registry.getPairByRFID(uuid1).track.c_str();
    //      audio_manager.updateTracks(nextAudioState);
    //      headEnabled = true;
    //    }
    //    if ((millis() - msAtLastCheck) > timeToWaitBeforeTestFitChecks * 2 && !topEnabled) {
    //      Serial.println(registry.getPairByRFID(uuid2).track.c_str());
    //      nextAudioState.top_track = registry.getPairByRFID(uuid2).track.c_str();
    //      audio_manager.updateTracks(nextAudioState);
    //      topEnabled = true;
    //    }
    //    if ((millis() - msAtLastCheck) > timeToWaitBeforeTestFitChecks * 3 && !bottomEnabled) {
    //      Serial.println("Starting bottom track");
    //      nextAudioState.bottom_track = registry.getPairByRFID(uuid3).track.c_str();
    //      audio_manager.updateTracks(nextAudioState);
    //      bottomEnabled = true;
    //    }
  }

}

// needs its own process so that it has no latency
void audioHandler(void *pvParameters) {
  for (;;) {
    audio_manager.audioLoop();
    delay(25);
  }
}

// RFID helpers vv

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}

void loop()
{

}

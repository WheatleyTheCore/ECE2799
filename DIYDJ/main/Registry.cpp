
#define FILENAME "/pairings.json"

#include "Registry.h"

static StaticJsonDocument<256> dock;

//define tracklist here
// doc["hello"] = "world";
doc["track1"] = "333";
doc["track2"] = "157";

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void writeJSON(StaticJsonDocument<256> doc) {
  File file = SD.open(FILENAME, FILE_WRITE);
  serializeJson(doc, file);
  file.close();
}

/**
  Takes in a key & JSON document, returns the value as a string
*/
const char* readJSON(char* key, StaticJsonDocument<256> doc) {
  File file = SD.open(FILENAME, FILE_READ);
  // Serial.println("entered read JSON");
  DeserializationError error = deserializeJson(doc, file);
  // Serial.println("passed error");
  const char* value = doc[key];
  file.close();
  return value;
}

struct RFIDTrackPair getPairByRFID(String RFID) {
    struct RFIDTrackPair pair;
    pair.RFID = RFID;
    pair.track = readJSON(RFID, doc);
    return pair;
}

Registry::initialize() {
    if(!SD.begin(5)){
        Serial.println("Card Mount Failed");
        return;
    } else {
        Serial.println("SD initialized");
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;  
    }

    writeJSON(dock); //writes JSON doc to file
}

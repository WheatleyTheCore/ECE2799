
#define FILENAME "/pairings.json"

#include "Registry.h"


//define tracklist here
// doc["hello"] = "world";


void Registry::readFile(fs::FS &fs,  String path){
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

void Registry::writeFile(fs::FS &fs,  String path,  String message){
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
//
//void deleteFile(fs::FS &fs, const String path){
//  Serial.printf("Deleting file: %s\n", path);
//  if(fs.remove(path)){
//    Serial.println("File deleted");
//  } else {
//    Serial.println("Delete failed");
//  }
//}

void Registry::writeJSONFile(JsonDocument doc) {
  File file = SD.open(FILENAME, FILE_WRITE);
  serializeJson(doc, file);
  file.close();
}

/**
  Takes in a key & JSON document, returns the value as a String
*/
String Registry::readJSON(String key, JsonDocument doc) {
  File file = SD.open(FILENAME, FILE_READ);
  // Serial.println("entered read JSON");
  DeserializationError error = deserializeJson(doc, file);
  // Serial.println("passed error");
  String value = doc[key];
  file.close();
  return value;
}

struct RFIDTrackPair Registry::getPairByRFID(String RFID) {
    struct RFIDTrackPair pair;
    pair.RFID = RFID;
    if (RFID != "") {
          pair.track = this->readJSON(RFID, dock);
    } else {
      pair.track = "";
    }
    return pair;
}

void Registry::initialize() {
//    if(!SD.begin(5)){
//        Serial.println("Card Mount Failed");
//        return;
//    } else {
//        Serial.println("SD initialized");
//    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;  
    }

    dock["head1"] = "head1";
    dock["top1"] = "top1";
    dock["bottom1"] = "bottom1";
    dock["head2"] = "head2";
    dock["top2"] = "top2";
    dock["bottom2"] = "bottom2";

    this->writeJSONFile(dock); //writes JSON doc to file
}

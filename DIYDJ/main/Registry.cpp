/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-microsd-card-arduino/
  
  This sketch can be found at: Examples > SD(esp32) > SD_Test
*/

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <ArduinoJson.h>

#define FILENAME "/pairings.json"

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

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

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

/**
  writes values from a json doc object into the defined file
*/
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

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial communication is working!");

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

  // JsonDocument doc;

  // Add values in the document


  StaticJsonDocument<256> doc;
  // doc["hello"] = "world";
  doc["track1"] = "333";
  doc["track2"] = "157";

  writeJSON(doc); //writes JSON doc to file 

  readFile(SD, FILENAME); //library function prints all contents of file
  Serial.println("");

  Serial.println("VALUES GO HERE");
  Serial.println(readJSON("track1", doc));
  Serial.println(readJSON("track2", doc));
  Serial.println("VALUES END HERE");
  
  deleteFile(SD, FILENAME);
}
void loop() {
  // Serial.println("hmm uhhh stpot");
  // sleep(2);
}
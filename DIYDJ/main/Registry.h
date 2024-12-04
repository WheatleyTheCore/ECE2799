#include <SD.h>
#include "FS.h"
#include "SPI.h"
#include <ArduinoJson.h>
#include "Arduino.h"

#define FILENAME "/pairings.json"
static JsonDocument dock;

struct RFIDTrackPair {
  String RFID;
  String track;
};


class Registry {
  public:
//    int loadPairsFromSD(SD)
    struct RFIDTrackPair getPairByRFID(String RFID);
    void initialize();
    // struct RFIDTrackPair setRFIDTrackPair(String RFID, String track); // does not need to be implemented for demo
    // int RFIDTrackPair removePairbyRFID(String RFID); // does not need to be implemented for demo
    
  private:
    void readFile(fs::FS &fs, String path);
    void writeFile(fs::FS &fs, String path, String message);
    void writeJSONFile(JsonDocument doc);
    String readJSON(String key, JsonDocument doc);
};

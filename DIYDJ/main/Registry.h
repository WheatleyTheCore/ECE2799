#include <SD.h>
#include "FS.h"
#include "SPI.h"
#include <ArduinoJson.h>

#define FILENAME "/pairings.json"

struct RFIDTrackPair {
  String RFID;
  String track;
}


class Registry {
  public:
    int loadPairsFromSD(SD)
    struct RFIDTrackPair getPairByRFID(String RFID);
    // struct RFIDTrackPair setRFIDTrackPair(String RFID, String track); // does not need to be implemented for demo
    // int RFIDTrackPair removePairbyRFID(String RFID); // does not need to be implemented for demo
    
  private:
    void readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void writeJSON(StaticJsonDocument<256> doc);
    const char* readJSON(char* key, StaticJsonDocument<256> doc);
}


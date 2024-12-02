#include <SD.h>

struct RFIDTrackPair {
  String RFID;
  String track;
}


class Registry {
  public:
    int loadPairsFromSD(SD)
    struct RFIDTrackPair getPairByRFID(String RFID);
    struct RFIDTrackPair setRFIDTrackPair(String RFID, String track); // does not need to be implemented for demo
    int RFIDTrackPair removePairbyRFID(String RFID); // does not need to be implemented for demo
}

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 21

struct ClothingState {
  String head;
  String top;
  String bottom;
};


class ClothingDetection {
  public:
    byte* getCurrentTags();
  private:
    void printHex(byte *buffer, byte bufferSize);
    void printDec(byte *buffer, byte bufferSize);
};

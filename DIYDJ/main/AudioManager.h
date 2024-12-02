
#include "Arduino.h"
#include "WiFiMulti.h"
#include "Audio.h"
#include "SD.h"
#include "FS.h"

// microSD Card Reader connections
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

struct AudioState
{
  String head_track;
  String top_track;
  String bottom_track;
}

class AudioManager
{
public:
  struct AudioState nextState;
  int initialize();
  struct AudioState updateTracks(AudioState newAudioState);
  void audioLoop();

private:
  Audio audio;
  struct AudioState currentState; // should technically be private with getter and setter function
}

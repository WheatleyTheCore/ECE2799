#include <Arduino.h>
#include "AudioManager.h"

AudioManager::initialize()
{
  Serial.begin(115200);
  // Set microSD Card CS as OUTPUT and set HIGH
  pinMode(SD_CS, OUTPUT); // FIXME may be an issue if nico also has to instantiate.
  digitalWrite(SD_CS, HIGH);

  // Initialize SPI bus for microSD Card
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Start Serial Port
  Serial.begin(115200);

  // Start microSD Card
  if (!SD.begin(SD_CS))
  {
    Serial.println("Error accessing microSD card!");
    while (true)
      ;
  }
  Serial.println("SD card initialized!");
  audio = new Audio(true, I2S_DAC_CHANNEL_BOTH_EN);
  currentState.head_track = "";
  currentState.top_track = "";
  currentState.bottom_track = "";
  nextState.head_track = "";
  nextState.top_track = "";
  nextState.bottom_track = "";
  return 1;
}

AudioManager::updateTracks(struct AudioState current, struct AudioState next)
{
  bool statesAreTheSame = curent.head_track = next.head_track &&curent.top_track = next.top_track &&curent.bottom_track = next.bottom_track;
  if (statesAreTheSame)
  {
    return current;
  } else {
    currentState = next;
  }
}

AudioManager::audioLoop() {
  audio.loop();
  if (!audio.isRunning()) {
    // we have hit the end of the loop!!!
    
  }
}

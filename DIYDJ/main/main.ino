#include <Audio.h>
#include "Arduino.h"
#include "SD.h"
#include "FS.h"
#include "Registry.h"
#include "AudioManager.h"

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

// microSD Card Reader connections
#define SD_CS          5
#define SPI_MOSI      23 
#define SPI_MISO      19
#define SPI_SCK       18

#define MS_BETWEEN_FIT_CHECKS 200 // @grace will need to be tuned to whatever seems to work well.


Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);
AudioManager audio_manager;
Registry registry;

AudioState nextAudioState = {"", "", ""};

unsigned long msAtLastCheck = 0; // ms elapsed since board startup, to be used in timing stuff (delay is blocking and since we're doing audio stuff we don't want that)
unsigned int timeToWaitBeforeTestFitChecks = 10000;
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
  registry.initialize();
  Serial.println("Registry initialized successfully!");

  audio_manager.updateTracks(nextAudioState);
  xTaskCreatePinnedToCore(mainBusinessLogic, "TaskCore0", 10000, NULL, 2, &TaskCore0, 0);
  xTaskCreatePinnedToCore(audioHandler, "TaskCore1", 10000, NULL, 1, &TaskCore1, 0);

}

void mainBusinessLogic(void *pvParameters) {
   for (;;) {

    // will need to be edited to check the fit, and if it's different update the tracks.
    
    if ((millis() - msAtLastCheck) > timeToWaitBeforeTestFitChecks && !headEnabled) {
      Serial.println(registry.getPairByRFID(uuid1).track.c_str());
      nextAudioState.head_track = registry.getPairByRFID(uuid1).track.c_str();
      audio_manager.updateTracks(nextAudioState);
      headEnabled = true;
    }
    if ((millis() - msAtLastCheck) > timeToWaitBeforeTestFitChecks * 2 && !topEnabled) {
      Serial.println(registry.getPairByRFID(uuid2).track.c_str());
      nextAudioState.top_track = registry.getPairByRFID(uuid2).track.c_str();
      audio_manager.updateTracks(nextAudioState);
      topEnabled = true;
    }
    if ((millis() - msAtLastCheck) > timeToWaitBeforeTestFitChecks * 3 && !bottomEnabled) {
      Serial.println("Starting bottom track");
      nextAudioState.bottom_track = registry.getPairByRFID(uuid3).track.c_str();
      audio_manager.updateTracks(nextAudioState);
      bottomEnabled = true;
    }
    delay(200);
   }

}

// needs its own process so that it has no latency
void audioHandler(void *pvParameters) {
  for (;;) {
      audio_manager.audioLoop();
      vTaskDelay(25);
  }
}

void loop()
{
  
}

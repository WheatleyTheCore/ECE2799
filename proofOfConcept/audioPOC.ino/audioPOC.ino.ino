
#include "Arduino.h"
#include "WiFiMulti.h"
#include "Audio.h"
#include "SD.h"
#include "FS.h"
 
// microSD Card Reader connections
#define SD_CS          5
#define SPI_MOSI      23 
#define SPI_MISO      19
#define SPI_SCK       18

Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);

void setup() {
   Serial.begin(115200);
   // Set microSD Card CS as OUTPUT and set HIGH
    pinMode(SD_CS, OUTPUT);      
    digitalWrite(SD_CS, HIGH); 
    
    // Initialize SPI bus for microSD Card
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    
    // Start Serial Port
    Serial.begin(115200);
    
    // Start microSD Card
    if(!SD.begin(SD_CS))
    {
      Serial.println("Error accessing microSD card!");
      while(true); 
    }
    Serial.println("SD card initialized!");
//    WiFi.mode(WIFI_STA);
//    wifiMulti.addAP(ssid.c_str(), password.c_str());
//    wifiMulti.run();
//    if(WiFi.status() != WL_CONNECTED){
//        WiFi.disconnect(true);
//        wifiMulti.run();
//    }
    audio.setVolume(20); // 0...21

    const char* audiofilenames[] = {"/top.mp3", "bottom.mp3"};

    audio.connecttoFS(SD, audiofilenames);  

    audio.setFileLoop(true);
    
    }

void loop()
{
    audio.loop();
}

void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}

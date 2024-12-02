

void setup()
{
  // put your setup code here, to run once:
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
}

void loop()
{
  // put your main code here, to run repeatedly:
}

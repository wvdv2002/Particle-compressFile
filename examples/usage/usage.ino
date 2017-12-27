// Example usage for compressFile library by wvdv2002.

#include "SdFat.h"
#include "compressFile.h"

// Pick an SPI configuration.
// See SPI configuration section below (comments are for photon).
#define SPI_CONFIGURATION 3
//------------------------------------------------------------------------------
// Setup SPI configuration.
#if SPI_CONFIGURATION == 0
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = SS;
#elif SPI_CONFIGURATION == 1
// Secondary SPI with DMA
// SCK => D4, MISO => D3, MOSI => D2, SS => D1
SdFat sd(1);
const uint8_t chipSelect = D1;
#elif SPI_CONFIGURATION == 2
// Primary SPI with Arduino SPI library style byte I/O.
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFatLibSpi sd;
const uint8_t chipSelect = SS;
#elif SPI_CONFIGURATION == 3
// Software SPI.  Use any digital pins.
// MISO => D5, MOSI => D6, SCK => D7, SS => D0
SdFatSoftSpi<D5, D6, D7> sd;
const uint8_t chipSelect = D0;
#endif  // SPI_CONFIGURATION
//------------------------------------------------------------------------------


// Initialize objects from the lib
CompressFile compressFile;
File myFile;

void setup() {
  Serial.begin(9600);
  // Wait for USB Serial
  while (!Serial) {
    SysCall::yield();
  }

  Serial.println("Compression library test");
  Serial.println("Type any character to start");
  while (Serial.read() <= 0) {
    SysCall::yield();
  }

  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }
  // open the file for write at end like the "Native SD library"
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
  }
  // if the file opened okay, write to it:
  Serial.print("Writing to test.txt...");
  for(int i = 0;i<100;i++){
    myFile.println("testing 1, 2, 3");
  }
  myFile.printf("fileSize: %d\n", myFile.fileSize());
  // close the file:
  myFile.close();
  Serial.println("done.");

  Serial.println("Compressing file");
  compressFile.startCompress("test.txt","test.txt.lz"); //This will overwrite old files.
}

void loop() {
    compressFile.task(); //Compress the file in 512 byte read chunks.
    if(compressFile.getState()==COMPRESS_DONE){
      Serial.println("Compression done");
      if(myFile.open("test.txt.lz", O_READ)){
        Serial.println(String("Size of file: ") + String(myFile.fileSize()));
      }else{
        Serial.println("Something went wrong, cannot find compressed file");
      }
      while(1);
    }
    if(compressFile.getError()!=COMPRESS_NOERROR){
      Serial.println("Error while compressing, state:" + compressFile.toString());
      while(1);
    }
}

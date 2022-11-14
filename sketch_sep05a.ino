/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-microsd-card-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>
String dataMessage2;
String dataMessage1;
// Initialize SD card
void initSDCard(){
   if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

// Write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card
void appendFile(fs::FS &fs, const char * path, String  message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
int led_state = LOW;
String content = "";
String PathUart1="/dataFromUart1.txt";
String PathUart2="/dataFromUart2.txt";
char character;
void setup() {
  Serial.begin(115200);
  Serial2.begin(921600);
  Serial1.begin(921600,SERIAL_8N1,32,33); // baud,data,rx,tx
  Serial2.setTimeout(100);
  Serial1.setTimeout(100);
   // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  initSDCard();
  
  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file_2 = SD.open("/dataFromUart2.txt");
  if(!file_2) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "//dataFromUart2.txt", "OriginGPS =] \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file_2.close();

  File file_1 = SD.open("/dataFromUart1.txt");
  if(!file_1) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "//dataFromUart1.txt", "OriginGPS =] \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file_1.close();
}

void loop() {
 

               
          dataMessage2=Serial2.readString();
          Serial.print("Saving data from UART 2: ");
          Serial.println(dataMessage2);
          appendFile(SD, PathUart2.c_str(),dataMessage2);

          dataMessage1=Serial1.readString();
          Serial.print("Saving data from UART 1: ");
          Serial.println(dataMessage1);
          appendFile(SD, PathUart1.c_str(),dataMessage1);
          

          
            // toggle state of LED
           
             led_state = !led_state;
            

    // control LED arccoding to the toggled state
    digitalWrite(LED_BUILTIN, led_state);
//          //Serial.println(dataMessage);
//          //Append the data to file
//          
   
    
     
}

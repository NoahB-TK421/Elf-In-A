/*Header 

Created by Noah Brown 1/1/2024

This code plays sound files for a prop phone for Santa. 
The product is still in development and will be updated as need  

*/

#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>

const int buttonPin = 0;  
const int buttonPin1 = 1;
const int buttonPin2 = 2;
const int LEDPin1 = 3;
const int LEDPin2 = 4;
const int LEDPin3 = 5;
int buttonState;
int count = 0;
int songIndex = 0;
int fileCount = 0;
String fileNames[0];
File myFiles[0];

void setup()
{
  //sets all pins to input/output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(LEDPin3, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin1, INPUT);

  //Check to see if the SD card is readable
  if (!SD.begin(SDCARD_SS_PIN)) {

    while(true){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(500);                      // wait
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
    };

  }

  // 44100kHz stereo => 88200 sample rate

  //AudioZero.begin(2*44100);
  
}

void readSDCard() {
  File root = SD.open("/"); // Open the root directory
    
  while (true) {
    File entry = root.openNextFile(); // Open the next file in root directory
    
    // If no more files, break the loop
    if (!entry) {
      Serial.println("End of files.");
      break;
    }
    
    // Check if the entry is a file
    if (!entry.isDirectory()) {
      // Get the file name and store it in the array
      fileNames[fileCount] = entry.name();
      Serial.print("File ");
      Serial.print(fileCount);
      Serial.print(": ");
      Serial.println(fileNames[fileCount]);
      
      fileCount++; // Increment file count
    }
    
    entry.close(); // Close the file
  }
  
  root.close(); // Close the root directory
}

//Checks to see if file has been correctly opened and porvides a unique error code on the built in LED
void checkFile(File tempFile, int delayTime){

  if (!tempFile) {

    // if the file didn't open, print an error and stop

    while(true){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(delayTime*1000);                      // wait 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);
    };

  }
}

//Opens all available files on SD card then checks that all the files are ready to use
void openFiles(){
  for(int i = 0; i <= fileCount; i++){
    myFiles[i] = SD.open(fileNames[i]);
  }
  for (int i = 0; i<= fileCount; i++){
    checkFile(myFiles[i],i+1);
  }

}

void loop()
{

  
  while(count == 0){

    openFiles();

    int currentFile = 0;

    //increment current file ready to be played 
    if (digitalRead(buttonPin) == LOW){
      currentFile++;
    }

    //decrement current file ready to be played 
    if (digitalRead(buttonPin1) == LOW){
      currentFile--;
    }

    //Plays current file until file is finished playing
    if (digitalRead(buttonPin2) == LOW){
      // until the file is not finished
      AudioZero.begin(2*44100);
      digitalWrite(LEDPin2, HIGH);
      AudioZero.play(myFiles[currentFile]);
      AudioZero.end();
      digitalWrite(LEDPin2, LOW);
    }

    //state to 
    if (digitalRead(buttonPin && buttonPin1 && buttonPin2) == HIGH){
      // until the file is not finished
      digitalWrite(LEDPin3, HIGH);
      //AudioZero.play(myFile2);
      delay(500);
      digitalWrite(LEDPin3, LOW);
    }
  }
}
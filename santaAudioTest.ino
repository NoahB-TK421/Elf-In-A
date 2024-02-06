/*Header 

Created by Noah Brown 1/1/2024

This code plays sound files for a prop phone for Santa. 
The product is still in development and will be updated as need  

*/

#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>


//Global variables
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
const int maxFiles = 9;
String fileNames[maxFiles];
File myFile, errorLog;

// Setup function - Initializes pins and checks SD card
void setup()
{
  //sets all pins to input/output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(LEDPin3, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);


  unsigned long startTime = millis();  // Get the current time in milliseconds

  //Check to see if the SD card is readable function will time out after 10 seconds
  while (!SD.begin(SDCARD_SS_PIN)) {
    //unsigned long startTime = millis();  // Get the current time in milliseconds

    SD.begin(SDCARD_SS_PIN); //retry initialization

    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(50);

    if (millis() - startTime > 10000) { //wait 10 seconds before timeing out
      
      //breathBuiltInLED(1);
      writeErrorToLog("Error initializing SD card");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      return;  // Exit the setup function
    }

  }

  // Open error log file
  errorLog = SD.open("error.log", FILE_WRITE);
  startTime = millis();  // Get the current time in milliseconds

  while (!errorLog) {
    //unsigned long startTime = millis();  // Get the current time in milliseconds
    // Error opening error log file
    
    errorLog = SD.open("error.log", FILE_WRITE); //retry opening file

    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(50);

    if (millis() - startTime > 10000) {
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

      //breathBuiltInLED(1);
      return;  // Exit the setup function
    }
  }
  writeErrorToLog("start of program");
  //AudioZero.begin(2*44100);
  
}

// Function to simulate breathing effect on built-in LED
void breathBuiltInLED(int num){

  for(int j = 0; j <= num; j++){
    //breath in
    for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(LED_BUILTIN, brightness); // Set the LED brightness
    delay(10); // Adjust the speed of the breathing effect by changing the delay time
    }

    // Breathe out
    for (int brightness = 255; brightness >= 0; brightness--) {
      analogWrite(LED_BUILTIN, brightness); // Set the LED brightness
      delay(10); // Adjust the speed of the breathing effect by changing the delay time
    }

  }

}

// Function to flash built-in LED
void flashBuiltInLED(int num){
  for(int i = 0; i<= num; i++){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(250);                      // wait
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(250);
  } 
} 

void writeErrorToLog(String errorMessage) {
  if (errorLog) {
    errorLog.println(errorMessage);
    errorLog.flush(); // Ensure data is written to the file
  }
}

/*Checks to see if file has been correctly opened and provides a unique error code on the built in LED
function will time out after the the first error code is shown
*/
void checkFile(File tempFile, int delayTime){
  unsigned long startTime = millis();  // Get the current time in milliseconds

  
  while (!tempFile) {
    // if the file didn't open, print an error and stop
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(delayTime*250);                      // wait 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(250);

    if (millis() - startTime > (1+delayTime)*250) {
      // Handle timeout here
      //breathBuiltInLED(1);
      writeErrorToLog(String(tempFile.name()) + " did not open");
      return;  // Exit the function
    }
  }
}

void readCSVFile(){

  String valueStr = "";

  File dataFile = SD.open("data.csv");
  
  if (!dataFile) {
    breathBuiltInLED(2);
    writeErrorToLog(String(dataFile.name()) + " is not open");
    return;
  }

  // Read the CSV file character by character
  while (dataFile.available()) {
    char c = dataFile.read();

    // Process the character
    if (c == ',') {
      // Found a comma, process the value before it
      fileNames[fileCount] = valueStr;
      // Clear valueStr for next value
      fileCount++;
      valueStr = "";
      flashBuiltInLED(1);
    } else if (c == '\n') {
      // Found a end of line, process the value before it
      fileNames[fileCount] = valueStr;
      // Clear valueStr for to save memory
      valueStr = "";
      flashBuiltInLED(1);
      break;
    } else {
      // Append the character to valueStr
      valueStr += c;
    }
  }
  dataFile.close();
}

//Opens available file on SD card 
//then checks that the file is ready to use
void openFile(String tempString, int tempCurrentFile){
  myFile.close();
  myFile = SD.open(tempString);
  checkFile(myFile,tempCurrentFile);
  breathBuiltInLED(1);
}

void loop()
{

  readCSVFile();

  int currentFile = 0;

  openFile(fileNames[currentFile], currentFile);
  
  while(count == 0){
    //check that the file is always available
    checkFile(myFile,currentFile);
    //increment current file ready to be played 
    if (digitalRead(buttonPin) == LOW){

      digitalWrite(LEDPin1, HIGH);
      if(currentFile < fileCount){
        currentFile++;
        openFile(fileNames[currentFile],currentFile);
      }
      digitalWrite(LEDPin1, LOW);
    }

    //decrement current file ready to be played 
    if (digitalRead(buttonPin1) == LOW){

      digitalWrite(LEDPin1, HIGH);
      if(currentFile > 0){
        currentFile--;
        openFile(fileNames[currentFile],currentFile);
      }
      digitalWrite(LEDPin1, LOW);
    }

    //Plays current file until file is finished playing
    if (digitalRead(buttonPin2) == LOW){
      // until the file is not finished

      // 44100kHz stereo => 88200 sample rate

      AudioZero.begin(2*44100);
      digitalWrite(LEDPin2, HIGH);
      AudioZero.play(myFile);
      AudioZero.end();
      openFile(fileNames[currentFile],currentFile);
      digitalWrite(LEDPin2, LOW);
    }

    //state to proved indacation that program is active but cause no sound
    if (digitalRead(buttonPin2) != LOW){
      // until the file is not finished
      digitalWrite(LEDPin3, HIGH);
      delay(50);
      digitalWrite(LEDPin3, LOW);
    }
  }
}
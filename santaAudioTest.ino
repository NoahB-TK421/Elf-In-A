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
File myFile;

// Setup function - Initializes pins and checks SD card
void setup()
{
  unsigned long startTime = millis();  // Get the current time in milliseconds
  //sets all pins to input/output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(LEDPin3, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  //Check to see if the SD card is readable function will time out after 10 seconds
  while (!SD.begin(SDCARD_SS_PIN)) {

    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(5000);                      // wait
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
    
    if (millis() - startTime > 10000) {
      // Handle timeout here 
      breathBuiltInLED(1);
      return;  // Exit the setup function
    }

  }

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
    delay(500);                      // wait
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
  } 
} 

/*Checks to see if file has been correctly opened and provides a unique error code on the built in LED
function will time out 5 seconds after the the first error code is shown
*/
void checkFile(File tempFile, int delayTime){
  unsigned long startTime = millis();  // Get the current time in milliseconds

  
  while (!tempFile) {
    // if the file didn't open, print an error and stop
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(delayTime*1000);                      // wait 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);

    if (millis() - startTime > (5+delayTime)*1000) {
      // Handle timeout here
      breathBuiltInLED(1);
      return;  // Exit the function
    }
  }
}

void readCSVFile(){

  String valueStr = "";

  File dataFile = SD.open("data.csv");
  
  if (!dataFile) {
    breathBuiltInLED(2);
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
void openFile(String tempString){
  myFile.close();
  myFile = SD.open(tempString);
  flashBuiltInLED(2);
  checkFile(myFile,1);
  breathBuiltInLED(1);
}

void loop()
{

  readCSVFile();

  int currentFile = 0;

  openFile(fileNames[currentFile]);
  
  while(count == 0){
    //check that the file is always available
    checkFile(myFile,1);
    //increment current file ready to be played 
    if (digitalRead(buttonPin) == LOW){
      //digitalWrite(LEDPin3, LOW);
      digitalWrite(LEDPin1, HIGH);
      currentFile++;
      openFile(fileNames[currentFile]);
      digitalWrite(LEDPin1, LOW);
    }

    //decrement current file ready to be played 
    if (digitalRead(buttonPin1) == LOW){
      //digitalWrite(LEDPin3, LOW);
      digitalWrite(LEDPin1, HIGH);
      currentFile--;
      openFile(fileNames[currentFile]);
      digitalWrite(LEDPin1, LOW);
    }

    //Plays current file until file is finished playing
    if (digitalRead(buttonPin2) == LOW){
      // until the file is not finished
      //digitalWrite(LEDPin3, LOW);
      // 44100kHz stereo => 88200 sample rate

      AudioZero.begin(2*44100);
      digitalWrite(LEDPin2, HIGH);
      AudioZero.play(myFile);
      AudioZero.end();
      digitalWrite(LEDPin2, LOW);
    }

    //state to proved indacation that program is active but cause no sound
    if (digitalRead(buttonPin2) != LOW){
      // until the file is not finished
      digitalWrite(LEDPin3, HIGH);
      //AudioZero.play(myFile2);
      delay(50);
      digitalWrite(LEDPin3, LOW);
    }
  }
}
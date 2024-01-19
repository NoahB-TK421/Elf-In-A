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
const int LEDPin1 = 2;
const int LEDPin2 = 3;
const int LEDPin3 = 4;
int buttonState;
int count = 0;
int songIndex = 0;


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

void loop()
{

  
  while(count == 0){

    // open wave file from sdcard
    File myFile = SD.open("test.wav");
    File myFile1 = SD.open("r2d2.wav");
    File myFile2 = SD.open("quit.wav");
    
    //Array containing all files expected to be played
    File myFiles[] = {myFile,myFile1,myFile2};
    
    //Checkes if all files are ready to be played
    for (int i = 0; i<=2; i++){
      checkFile(myFiles[i],i+1);
    }

    if (digitalRead(buttonPin) == LOW){
      // until the file is not finished
      AudioZero.begin(2*44100);
      digitalWrite(LEDPin1, HIGH);
      AudioZero.play(myFile);
      AudioZero.end();
      digitalWrite(LEDPin1, LOW);
    }

    if (digitalRead(buttonPin1) == LOW){
      // until the file is not finished
      AudioZero.begin(2*44100);
      digitalWrite(LEDPin2, HIGH);
      AudioZero.play(myFile1);
      AudioZero.end();
      digitalWrite(LEDPin2, LOW);
    }

    if (digitalRead(buttonPin && buttonPin1) == HIGH){
      // until the file is not finished
      digitalWrite(LEDPin3, HIGH);
      //AudioZero.play(myFile2);
      delay(500);
      digitalWrite(LEDPin3, LOW);
    }
  }
}
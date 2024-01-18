#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>

const int buttonPin = 0;  
const int buttonPin1 = 1;
const int buttonPin2 = 2;
const int buttonPin3 = 3;
const int buttonPin4 = 4;
int buttonState;
int count = 0;
int songIndex = 0;

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin2, OUTPUT);
  pinMode(buttonPin3, OUTPUT);
  pinMode(buttonPin4, OUTPUT);

  if (!SD.begin(SDCARD_SS_PIN)) {

    //Serial.println(" failed!");

    while(true){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(500);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
    };

  }

  // 44100kHz stereo => 88200 sample rate

  AudioZero.begin(2*44100);

  pinMode(buttonPin, INPUT);
  pinMode(buttonPin1, INPUT);
  
}

void loop()
{
while(count == 0){

// open wave file from sdcard
  
  File myFile = SD.open("test.wav");
  File myFile1 = SD.open("r2d2.wav");
  File myFile2 = SD.open("quit.wav");


  if (!myFile) {

    // if the file didn't open, print an error and stop

    while(true){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(2000);                      // wait 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);
    };

  }

  if (!myFile1) {

    // if the file didn't open, print an error and stop

    while(true){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(3000);                      // wait
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait 
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);
    };

  }

 if (!myFile2) {

   // if the file didn't open, print an error and stop

   while(true){
   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
   delay(4000);                      // wait
   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
   delay(1000);
   };

 }

  if (digitalRead(buttonPin) == LOW){
    // until the file is not finished
    digitalWrite(buttonPin2, HIGH);
    AudioZero.play(myFile);
    digitalWrite(buttonPin2, LOW);
  }

  if (digitalRead(buttonPin1) == LOW){
    // until the file is not finished
    digitalWrite(buttonPin3, HIGH);
    AudioZero.play(myFile1);
    digitalWrite(buttonPin3, LOW);
  }

  if (digitalRead(buttonPin && buttonPin1) == HIGH){
    // until the file is not finished
    digitalWrite(buttonPin4, HIGH);
    AudioZero.play(myFile2);
    digitalWrite(buttonPin4, LOW);
  }
}}
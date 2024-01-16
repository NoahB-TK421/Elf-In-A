#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>

const int buttonPin = 0;  
const int buttonPin1 = 1;
int buttonState;
int count = 0;

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);

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

  switch (digitalRead(buttonPin)) {
    case LOW:
      playAudio(buttonPin, myFile);
      break;
    case HIGH:
      // Check the second buttonPin
      switch (digitalRead(buttonPin1)) {
        case LOW:
          playAudio(buttonPin1, myFile1);
          break;
        case HIGH:
          // Check both buttons
          if (digitalRead(buttonPin) == HIGH && digitalRead(buttonPin1) == HIGH) {
            // until the file is not finished
            AudioZero.play(myFile2);
            digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
            delay(500);                      // wait
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
            delay(500);
          }
          break;
      }
      break;
  }
}

  //if (digitalRead(buttonPin) == LOW){
  //  // until the file is not finished
  //  AudioZero.play(myFile);
  //}
//
  //if (digitalRead(buttonPin1) == LOW){
  //  // until the file is not finished
  //  AudioZero.play(myFile1);
  //}
//
  //if (digitalRead(buttonPin && buttonPin1) == HIGH){
  //  // until the file is not finished
  //  AudioZero.play(myFile2);
  //  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //  delay(500);                      // wait
  //  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //  delay(500);
  //}
  }
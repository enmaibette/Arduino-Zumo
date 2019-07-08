/*
  Timer with blink LED

  It's a countdown which turns the LED's on and off for several seconds, when it reaches zero.

  modified 2 June 2019
  by Lara Akgün

  This example code is from the arduino une multifunction shield documentation.
*/

#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

// initialize the variables
enum CountDownModeValues {
  COUNTING_STOPPED,
  COUNTING
};

byte countDownMode = COUNTING_STOPPED;
byte tenths = 0;
char seconds = 0;
char minutes = 0;

// Setup of the application
void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1); // initialize multi-function shield library
  MFS.write(0);
  Serial.begin(9600);
}

// main code, which run repeatedly
void loop() {
  byte btn = MFS.getButton();
  switch (countDownMode) {
    case COUNTING_STOPPED:
      if (btn == BUTTON_1_SHORT_RELEASE && (minutes + seconds) > 0) {
        // start the timer
        countDownMode = COUNTING;
      } else if (btn == BUTTON_1_LONG_PRESSED) {
        // reset the timer and puts all to zero
        tenths = 0;
        seconds = 0;
        minutes = 0;
        MFS.write(minutes * 100 + seconds);
      }

      // Add minutes if button 2 is pressed long or short
      else if (btn == BUTTON_2_PRESSED || btn == BUTTON_2_LONG_PRESSED) {
        minutes++;
        if (minutes > 60) {
          minutes = 0;
        }
        MFS.write(minutes * 100 + seconds);
        // Add seconds if button 3 is pressed long or short
      } else if (btn == BUTTON_3_PRESSED || btn == BUTTON_3_LONG_PRESSED) {
        seconds += 10;
        if (seconds >= 60) {
          seconds = 0;
        }
        MFS.write(minutes * 100 + seconds);
      }
      break;

    case COUNTING:
      if (btn == BUTTON_1_SHORT_RELEASE || btn == BUTTON_1_LONG_RELEASE) {
        // stop the timer
        countDownMode = COUNTING_STOPPED;
      }
      else {
        // continue counting down
        tenths++;

        if (tenths == 10) {
          tenths = 0;
          seconds--;

          if (seconds < 0 && minutes > 0) {
            seconds = 59;
            minutes--;
          }

          // timer has reached 0, so sound the alarm and the LED begin to blink
          if (minutes == 0 && seconds == 0) {
            MFS.beep(50, 50, 5); // beep 3 times, 500 milliseconds on / 500 off
            MFS.writeLeds(LED_ALL, ON);
            MFS.blinkLeds(LED_ALL, ON);
            delay(2000);
            MFS.blinkLeds(LED_ALL, OFF);
            MFS.write("END");
            MFS.blinkLeds(LED_1, ON);
            delay(2000);
            MFS.blinkLeds(LED_1, OFF);
            MFS.blinkLeds(LED_2, ON);
            delay(2000);
            MFS.blinkLeds(LED_2, OFF);
            MFS.blinkLeds(LED_3, ON);
            delay(2000);
            MFS.blinkLeds(LED_3, OFF);
            MFS.blinkLeds(LED_4, ON);
            delay(2000);
            MFS.blinkLeds(LED_4, OFF);
            countDownMode = COUNTING_STOPPED;
          }
          MFS.write(minutes * 100 + seconds);
        }
        delay(100);
      }
      break;
  }
}

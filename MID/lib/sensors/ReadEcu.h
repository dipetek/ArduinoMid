//
// Created by Angel Zaprianov on 25.7.2016 г..
//

#ifndef ARDUINOMID_READECU_H
#define ARDUINOMID_READECU_H

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setupECU() {
/*  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }*/


//  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);

}

void readEcu() { // run over and over
//  if (mySerial.available()) {
//      Serial.write(mySerial.read());
//    }
//  if (Serial.available()) {
//      mySerial.write(Serial.read());
//    }
}
#endif //ARDUINOMID_READECU_H

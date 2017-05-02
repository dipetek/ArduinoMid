//
// Created by Angel Zaprianov on 11.10.2016 г..
//
#include <Arduino.h>
#include <avr/pgmspace.h>

#if SCREEN == 162 || !defined(SCREEN)

#import <LiquidCrystal.h>

#else

#endif

#ifndef SHUTDOWN_SAVE_STATE
#define SHUTDOWN_SAVE_STATE HIGH
#endif
#ifndef SHUTDOWN_SAVE_BUTTON
#define SHUTDOWN_SAVE_BUTTON 8
#endif

#ifndef SHUTDOWN_SAVE_LOOPS
#define SHUTDOWN_SAVE_LOOPS 1200
#endif

#ifndef SHUTDOWN_LOW_VALUE
#define SHUTDOWN_LOW_VALUE 500
#endif

#ifndef ARDUINOMID_SHUTDOWN_H
#define ARDUINOMID_SHUTDOWN_H

class ShutDw {


    AmpTime *amp;

    EepRom *eep;

    CarSens *car;

    WhlSens *whl;

private :
    unsigned int indexWait = 0;
    int detectorValue = 1000;
    uint8_t pinCtrl, pinDtct, pinTone;
    uint8_t alreadyShutdown = 0;
    uint8_t entryDisplay = 0;
    boolean isShutdownActive = 0;
    boolean alreadySaved = 0;
#if SCREEN == 162 || !defined(SCREEN)

/**
 * Cancel state
 */
    void doTripSkip() {
        //
        // Erase trip data
        eep->clearTripData();
        //
        // Shutdown the system
        tone(pinTone, 800, 500);
        whl->shutdownMode();
        delay(2000);
        analogWrite(pinCtrl, LOW);
        //
        // Mark mid as shutdown
        alreadyShutdown = 1;
    }


/**
 * Save state
 */
    void doTripSaved() {
        //
        // Mark saved
        alreadySaved = 1;

        //
        // Shutdown the system
        melodySave();
        whl->shutdownMode();
        delay(2000);
        digitalWrite(pinCtrl, LOW);
        //
        // Mark mid as shutdown
        alreadyShutdown = 1;
    }

#else

#endif

    void melodySave(void);

    char buffer[74];




public:
    static constexpr uint8_t MENU_SHUTDOWN = 99;

    void menu(LcdMenuInterface *lcd);
/**
 * Constructor of shutdown
  * @param eepRom
  * @param ampInt
  * @param carSens
  * @param whlSens
  */
    ShutDw(EepRom &eepRom, AmpTime &ampInt, CarSens &carSens, WhlSens &whlSens) :
            eep(&eepRom), amp(&ampInt), car(&carSens), whl(&whlSens) {
    }

    void setup(uint8_t pinControl, uint8_t pinDetect, uint8_t pinToAlarm);

    void listener();

    void cursor();

};


/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/




/**
 * Setup shutdown class
 */
void ShutDw::setup(uint8_t pinControl, uint8_t pinDetect, uint8_t pinToAlarm) {

    pinCtrl = uint8_t(pinControl);
    pinDtct = uint8_t(pinDetect);
    pinTone = uint8_t(pinToAlarm);


    //
    // Control save shutdown
    pinMode(pinCtrl, OUTPUT);
    analogWrite(pinCtrl, 255);
    //
    // Detect ignition key
    pinMode(pinDtct, INPUT);

}


/**
 * Handler the cursor menu
 */
void ShutDw::cursor() {

    //
    // Switch menu if cannot shutdown ...
    if (alreadyShutdown == 1) {
        MidCursorMenu = 1;
        alreadyShutdown = 2;

    }

    if (isShutdownActive && alreadyShutdown != 2) {
        MidCursorMenu = MENU_SHUTDOWN;
    }

}

/**
 * Listen shutdown and change menu to shutdown
 */
void ShutDw::listener() {
    //
    // Get voltage from pin
    detectorValue = analogRead(pinDtct);


//    if (amp->isMid()) {
//        Serial.print("Detected car voltage is: ");
//        Serial.println(detectorValue);
//    }

    if (detectorValue < SHUTDOWN_LOW_VALUE && alreadyShutdown != 2) {
        //
        // get data again
        detectorValue = analogRead(pinDtct);

        //
        // Is shutdown mode .... not noise
        if (amp->isMax() && detectorValue < SHUTDOWN_LOW_VALUE) {
            isShutdownActive = true;
        }
        //
        // Check for some data changed,  but in case save button is pressed ... shutdown save trigger ...
        if (amp->isMax() && detectorValue < SHUTDOWN_LOW_VALUE && !car->isRunEng()) {
            //
            // Fixes digital pot
            whl->shutdownMode();
            //
            // Wait dig pot
            delay(100);
            //
            // Skip save and shutdown
            digitalWrite(pinCtrl, LOW);
        }
        //
        // Reverse value for next check
        detectorValue = !detectorValue;
    }


}

/**
 * Save melody play
 */
void ShutDw::melodySave() {
    tone(pinTone, 1000, 50);
    delay(50);
    tone(pinTone, 1500, 50);
    delay(50);
    tone(pinTone, 1500, 50);
    delay(50);
    tone(pinTone, 2000, 50);
    delay(50);
    tone(pinTone, 3000, 50);
}

#if SCREEN == 162 || !defined(SCREEN)

/**
 * Display shutdown menu
 */
void ShutDw::menu(LcdMenuInterface *lcd) {

    //
    // Basic information save
    if (car->getDst() < SHUTDOWN_SAVE_TRIP|| true) { // TODO disabled for now
        tone(pinTone, 1000, 50);
        delay(50);
        tone(pinTone, 1200, 50);
        delay(50);

        lcd->drawShutdownShort();

        melodySave();
        eep->saveCurrentData();
        whl->shutdownMode();
        delay(2200);
        digitalWrite(pinCtrl, LOW);
        return;
    }

    char sec[2];
    //
    // Show message before straiting procedure of shouting down
    if (entryDisplay == 0) {

        //
        // Let LCD driver do this draw
        lcd->drawShutdownBegin();

        tone(pinTone, 800, 200);
        delay(200);
        tone(pinTone, 800, 200);
        delay(200);
        tone(pinTone, 3000, 1000);
        delay(3600);
        tone(pinTone, 2000, 600);
        delay(1000);
        entryDisplay = 1;
    }


    //
    // Catch seconds from loop
    if (amp->isSec()) {
        //
        // Convert data to human format
        sprintf(sec, "%02d", ((indexWait - SHUTDOWN_SAVE_LOOPS) / 200) * -1);
        lcd->drawShutdownCount(sec);
    }

    //
    // Listen press button
    if (digitalRead(SHUTDOWN_SAVE_BUTTON) == SHUTDOWN_SAVE_STATE) {
        delay(5);
        if (digitalRead(SHUTDOWN_SAVE_BUTTON) == SHUTDOWN_SAVE_STATE && alreadySaved == 0) {
            lcd->draWShutdownTripSkip();
            doTripSkip();
        }
    }
    //
    // Shutdown without wait ...
    if (indexWait >= SHUTDOWN_SAVE_LOOPS && alreadyShutdown == 0) {
        //
        // Save current data and shutdown
        eep->saveTripData();
        //
        // Show on playEntry
        lcd->draWShutdownTripSave();
        doTripSaved();
    }
    //
    // Count loops
    indexWait++;

}



#endif

#endif //ARDUINOMID_SHUTDOWN_H

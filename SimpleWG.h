#ifndef _SIMPLEWG_H
#define _SIMPLEWG_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

class SimpleWG {

  private:
    /* Le buffer */
    int *buffer;

    /* Last bit received time, packet gap, and bit tracker */
    unsigned long lastBitReceivedAt;
    int wgPacketGap;
    int currentBit;

    /* Pins */
    int wg0Pin;
    int wg1Pin;

  public:
    /* Initializer */
    bool start(uint8_t wg0, uint8_t wg1, unsigned int bits, unsigned int packetGap = 25);

    /* Interrupt function to be called on Wiegand pin state changes */
    void wgInterrupt();

    /* Whether we have something or not */
    bool available();

    /* The "raw" int array payload */
    int *payload();

    /* Print payload out nicely via serial */
    void printData(bool withNewLine = false);

    /* How many "bits" do we have? */
    int bits();

    /* Rinse and repeat */
    void clearBuffer();
};

#endif

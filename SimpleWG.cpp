#include "SimpleWG.h"

/* Le buffer */
int *buffer;

/* When did we get our last bit? */
unsigned long lastBitReceivedAt = 0;

/* What's the "gap" to consider this transmission done? */
int wgPacketGap;

/* What bit are we currently at? */
int currentBit = 0;

/* How many bits we can take, maximum? */
int maxBits = 0;

/* What is the actual size of our buffer? */
int bufferSize = 0;

/**
 * This checks if data is available
 */
bool SimpleWG::available() {
    /* First, do we have anything at all? */
    if (currentBit == 0) {
        return false;
    }

    /* Last bit received at must not be 0 */
    if (lastBitReceivedAt == 0) {
        return false;
    }

    /* Have we passed the "end-of-transmission" packet gap threshold? */
    unsigned long threshold = lastBitReceivedAt + wgPacketGap;
    if (threshold > millis()) {
        return false;
    }

    /* Whatever remaining, however improbable, must be true */
    return true;
}

/**
 * This returns raw payload, just in case someone wants to use that
 */
int *SimpleWG::payload() { return buffer; }

/**
 * Print out data via serial
 */
void SimpleWG::printData(bool withNewLine = false) {
    for (int i = 0; i < currentBit; i++) {
        Serial.print(buffer[i], DEC);
    }
    if (withNewLine) {
        Serial.println();
    }
}

/**
 * This resets the state of the buffer (so we'd be ready to grab new data!)
 */
void SimpleWG::clearBuffer() {
    currentBit = 0;
    lastBitReceivedAt = 0;
    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = -1;
    }
}

/**
 * How many bits do we have?
 */
int SimpleWG::bits() { return currentBit; }

/**
 * Wiegand read handler
 */
void SimpleWG::wgInterrupt() {

    /* What bit are we getting? */
    int bit = -1;

    /* Read states of pins wg0 and wg1 */
    int wg0State = digitalRead(wg0Pin);
    int wg1State = digitalRead(wg1Pin);

    /*
     Wiegand format:
        Both HIGH -> ignore
        D0 LOW, D1 HIGH -> "0"
        D0 HIGH, D1 LOW -> "1"
    */
    if (wg0State == LOW && wg1State == HIGH) {
        bit = 0;
    } else if (wg0State == HIGH && wg1State == LOW) {
        bit = 1;
    }

    /* If bit is valid (i.e. bit != -1), we push into buffer */
    if (bit != -1) {
        /* Only do this if we are sure there will be no overflow */
        if (currentBit <= maxBits) {
            buffer[currentBit] = bit;
            currentBit++;
        }
    }

    /* Set last bit received */
    lastBitReceivedAt = millis();
}

/**
 * Start SimpleWG instance
 */
bool SimpleWG::start(uint8_t wg0, uint8_t wg1, unsigned int bits, unsigned int packetGap = 25) {

    /* Allocate primary buffer space */
    bufferSize = (bits * sizeof(int)) + 1;
    buffer = (int *)malloc(bufferSize);
    if (buffer == NULL) {
        /* Cannot allocate memory */
        return false;
    }

    /* We first want to set both our pins as input */
    pinMode(wg0, INPUT);
    pinMode(wg1, INPUT);
    wg0Pin = wg0;
    wg1Pin = wg1;

    /* Set packet gap and maximum bits */
    wgPacketGap = packetGap;
    maxBits = bits;

    /* Clear buffer */
    clearBuffer();

    return true;
}

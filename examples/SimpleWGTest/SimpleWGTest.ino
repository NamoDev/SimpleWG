#include <SimpleWG.h>

/* Instantiate SimpleWG */
SimpleWG reader0;

/* Due to how Arduino handles interrupts, we need to create a "wrapper" here */
void r0_interrupt() { reader0.wgInterrupt(); }

void setup() {
    Serial.begin(115200);

    /* Wiegand D0 and D1 pins */
    int d0 = 14;
    int d1 = 15;

    /* Maximum Wiegand bits to support */
    int maxBits = 48;

    /* Packet gap (ms) - determines how long the "burst" must end before we
     * confirm a "packet" */
    int packetGap = 25;

    /* Initialize this instance of SimpleWG */
    if (!reader0.start(d0, d1, maxBits, packetGap)) {
        Serial.println("ERROR - unable to allocate memory!");
    }

    /* Attach pin interrupts */
    attachInterrupt(digitalPinToInterrupt(d0), r0_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(d1), r0_interrupt, CHANGE);

    Serial.println("SimpleWG example ready :)");
}

void loop() {
    /* We check if there is a wiegand "packet" */
    if (reader0.available()) {

        /* .bits() gets us the number of bits in the Wiegand message */
        Serial.print("Got data (");
        Serial.print(reader0.bits());
        Serial.println(" bits)");

        /* Boolean in printData() tells SimpleWG we want a newline also */
        reader0.printData(true);

        /* Remember to clear buffer when done, it's important! */
        reader0.clearBuffer();
    }
}

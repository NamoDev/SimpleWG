# SimpleWG
A really simple Wiegand library for Arduino

### Features
* Works with any arbitrary Wiegand bit length
* Multi-instance support. Connect independently to as many Wiegand readers as you want! (subject to Arduino memory and pin limits, of course)
* (Hopefully) simple enough to understand

### Testing
This has been tested on an Arduino Nano Every, in conjunction of the following readers:

* HID iClass SE RK40
* HID iClass SE R40
* Generic 26-bit EM card reader

```
Got data on HID iClass SE R40 (R0) (32 bits)
10001010011101100111101100000100

Got data on HID iClass SE RK40 (R1) (4 bits)
0001

Got data on 26-bit EM reader (R2) (26 bits)
10111000100011111101100101
```

### TODO
* Optimize code for style
* Triple-check for memory issues (I don't typically code in C++, so...)

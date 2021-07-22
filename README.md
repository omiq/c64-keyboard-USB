# c64-keyboard-USB
Arduino Pro Micro C64 Keyboard to USB Adapter

1. First version is a modification of firmware **originally by DJ Sures** (Synthiam.com) (c)2019 that simply fixes the cursor up to work using both left AND right shift
1. Second version changes the C= and CTRL key behaviour, and makes C= + F7 into F12 for my preference
1. The third version is a (work in progress) version that will be detected by bare-metal/FPGA such as MiSTer, BMC64 and Ultimate64 as well as Vice. You will need the [HID](https://github.com/NicoHood/HID) library for Arduino.


### Uncomment the correct target machine to use your C64 keyboard with

//#define U64
//#define VICE
//#define MISTER
//#define BMC64



```

 ----------------------
 CONNECTIONS
 ----------------------

     C64   |  Arduino
    ==================
       20     2 - SDA
       19     3 - SCL
       18     4 - A6
       17     5
       16     6 - A7
       15     7 -
       14     8 - A8
       13     9 - A9
       12     10 - A10
       11     16 - MOSI
       10     14 - MISO
       9      15 - SCLK
       8      18 - A0
       7      19 - A1
       6      20 - A2
       5      21 - A3
       4      -N/C-
       3      1
       2      -N/C-
       1      0
    ==================

    

        ------------------------------------
        Commodore 64 keyboard matrix layout                                                                
        ------------------------------------
                                                                
        Del      Return   curl/r   F7       F1       F3       F5       curup
        3        W        A        4        Z        S        E        lSh
        5        R        D        6        C        F        T        X
        7        Y        G        8        B        H        U        V
        9        I        J        0        M        K        O        N
        +        P        L        –        .        :        @        ,
        £        *        ;        Home     rshift   =        ↑        /
        1        ←        Ctrl     2        Space    C=       Q        Stop

// Uncomment if you wish to debug keyboard through serial monitor at 115200 baud
//#define DEBUG
```

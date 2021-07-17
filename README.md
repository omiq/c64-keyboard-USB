# c64-keyboard-USB
Arduino Pro Micro C64 Keyboard to USB Adapter

First version simply fixes the cursor up to work using both left AND right shift

Second version changes the C= and CTRL key behaviour, and makes C+ + F7 into F12 for my preference

Originally by DJ Sures (Synthiam.com) (c)2019 

```// ** Commodore Key: Shortcut for F12 to bring VICE menu                **
// ** CTRL-SHIFT-W: Shortcut for Alt-w (vice warp mode)                 **
// **                                                                   **
// ***********************************************************************

// C64      Arduino
// KeyBoard Pro Micro
//  Pin     Pin
// Rows
//   20     2 - SDA
//   19     3 - SCL
//   18     4 - A6
//   17     5
//   16     6 - A7
//   15     7 -
//   14     8 - A8
//   13     9 - A9

// Columns
//   12     10 - A10
//   11     16 - MOSI
//   10     14 - MISO
//   9      15 - SCLK
//   8      18 - A0
//   7      19 - A1
//   6      20 - A2
//   5      21 - A3
//   4      N/C
//   3      1
//   2      N/C
//   1      0

// Uncomment if you wish to debug keyboard through serial monitor at 9600 baud
//#define DEBUG```

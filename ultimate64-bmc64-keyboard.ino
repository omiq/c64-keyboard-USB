
// Uncomment to debug using serial monitor at 115200 baud
//#define DEBUG

#include "HID-Project.h"

// ***********************************************************************
//                                                                    
//  Commodore 64 USB HID using Pro Micro by Chris Garrett @makerhacks  
//                                                                    
//  Inspired by and based on code by DJ Sures (Synthiam.com)          
//
// ***********************************************************************

// C64      Arduino

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

int  keyPos;
int  outChar;
bool isKeyDown;
int lastKeyState[80];
long lastDebounceTime[80];
int debounceDelay = 50;
int RowPinMap[8] = {9, 3, 4, 5, 6, 7, 8, 2};
int ColPinMap[10] = {10, 16, 14, 21, 18, 19, 20, 15, 1, 0};

// KEYBOARD LAYOUT
/*

Commodore 64 keyboard matrix layout                                                                
                                                                
        Del      Return   curl/r   F7       F1       F3       F5       curup
        3        W        A        4        Z        S        E        lSh
        5        R        D        6        C        F        T        X
        7        Y        G        8        B        H        U        V
        9        I        J        0        M        K        O        N
        +        P        L        –        .        :        @        ,
        £        *        ;        Home     rshift   =        ↑        /
        1        ←        Ctrl     2        Space    C=       Q        Stop


*/

uint16_t keyMap[80] = {
  // Del            Return      LR                F7        F1               F3             F5          UD               Null         Null
  KEY_BACKSPACE, KEY_RETURN, KEY_RIGHT_ARROW,  KEY_F7,   KEY_F1,          KEY_F3,        KEY_F5,     KEY_DOWN_ARROW,  NULL,        NULL,

      // 3           W           A                 4         Z                S              E           LSHFT            NULL     Joy1Down
      '3',           'w',        'a',              '4',      'z',             's',           'e',        KEY_LEFT_SHIFT,  '2',         '*',

      // 5           R           D                 6         C                F              T           X                Joy2Left     Joy1Left
     '5',           'r',        'd',              '6',      'c',             'f',           't',        'x',             '4',         '-',

      // 7           Y           G                 8         B                H              U           V                Joy2Right    Joy1Right
      '7',           'y',        'g',              '8',      'b',             'h',           'u',        'v',             '6',         '+',

      // 9           I           J                 Zero      M                K              O           N                Joy2F1       Joy1F1
      '9',           'i',        'j',              '0',      'm',             'k',           'o',        'n',             '5',         KEY_TAB,

      // +           P           L                 -         .                :              @           ,                Joy2F2       Joy1F2
      '+',           'p',        'l',              '-',      '.',             ':',           '@',        ',',             '3',         '1',

      // Pound       *           ;                 Home      RSHFT            =              ↑          /                Restore      Null
HID_KEYBOARD_LEFT_GUI,'*',      ';',              KEY_HOME, KEY_LEFT_SHIFT,  '=',           KEY_DELETE, '/',             '\\',         NULL,

      //  1          BS          CTRL              2         SPC              C=             Q           RunStop          Joy2Up       Joy1Up
      '1',           KEY_ESC,    KEY_LEFT_CTRL,    '2',      ' ',     HID_KEYBOARD_LEFT_GUI,'q',        KEY_ESC,         '8',         '\\',
};

void bootsetup() {

  Serial.begin(115200);
  BootKeyboard.begin();

}  

void special_keys() {
  
        // Special Keys
          switch (keyPos) 
          {

          case 72:
          #ifndef DEBUG
            BootKeyboard.press(KEY_TAB);
            delay(300);
            outChar = NULL;
          #endif   
            Serial.println("CTRL");           
            break;
    
          case 77:
          #ifndef DEBUG
            BootKeyboard.press(KEY_ESC);
            delay(300);
            outChar = NULL;
          #endif   
            Serial.println("RUNSTOP");
            break;
    
          case 75:
          #ifndef DEBUG
            BootKeyboard.press(MOD_LEFT_ALT);
            delay(300);
            outChar = NULL;
          #endif   
            Serial.println("C="); 
            break;
    
          case 1:
          #ifndef DEBUG
            BootKeyboard.write(KEY_RETURN);
            delay(300);
            outChar = NULL;
          #endif   
            Serial.println("RETURN");           
            break;
    
          case 0:
          #ifndef DEBUG
            outChar = NULL;
            BootKeyboard.press(KEY_BACKSPACE);
            delay(300);
          #endif   
            Serial.println("DEL");           
            break;
    
          case 63:
          #ifndef DEBUG
            BootKeyboard.press(KEY_HOME);
            delay(300);
            outChar = NULL;
          #endif   
            Serial.println("HOME");
            break;

           case 2:
            if (lastKeyState[17] || lastKeyState[64] ) 
            {
          #ifndef DEBUG
              outChar = NULL;
              BootKeyboard.write(KEY_LEFT);  
              delay(300);
          #endif   
              Serial.println("CURSOR LEFT");  
            } 
            else 
            {
          #ifndef DEBUG
              outChar = NULL;
              BootKeyboard.write(KEY_RIGHT);  
              delay(300);          
          #endif   
              Serial.println("CURSOR RIGHT");   
            }
            break;
            
            case 7:
            outChar = NULL;
            if (lastKeyState[17] || lastKeyState[64] ) 
            {
          #ifndef DEBUG
              BootKeyboard.write(KEY_UP);  
              delay(300);
              Serial.println("CURSOR UP");   
          #endif   
            } 
            else 
            {
          #ifndef DEBUG
              BootKeyboard.write(KEY_DOWN);  
              delay(300);
              Serial.println("CURSOR DOWN");              
          #endif   
            }      
            break;
    
          }
        

  
         // Shifted / modifiers (eg. shift-2 = ")
         if (lastKeyState[17] || lastKeyState[64] ) {
      
            switch (outChar) {
              
            case '1':
              outChar = '!';
              break;
      
            case '2':
              outChar = '"';
              break;
       
      
            case '3':
              outChar = '#';
              break;
       
      
            case '4':
              outChar = '$';
              break;
       
      
            case '5':
              outChar = '%';
              break;
       
      
            case '6':
              outChar = '&';
              break;
       
      
            case '7':
              outChar = '\'';
              break;
       
      
            case '8':
              outChar = '(';
              break;
       
      
            case '9':
              outChar = ')';
              break;
       
      
            case ':':
              outChar = '[';
              break;
       
      
            case ';':
              outChar = ']';
              break;
       
      
            case ',':
              outChar = '<';
              break;
       
      
            case '.':
              outChar = '>';
              break;
       
      
            case '/':
              outChar = '?';
              break;
       
          }
       }
}       

void setup() {

  bootsetup();

  for (int i = 0; i < 80; i++)
    lastKeyState[i] = false;

  for (int Row = 0; Row < 8; Row++)
    pinMode(RowPinMap[Row], INPUT_PULLUP);

  for (int Col = 0; Col < 10; Col++)
    pinMode(ColPinMap[Col], INPUT_PULLUP);
}

void loop() {

  keyPos    = NULL;
  outChar   = NULL;
  isKeyDown = NULL;


  for (int Row = 0; Row < 8; Row++) {

    int RowPin = RowPinMap[Row];
    pinMode(RowPin, OUTPUT);
    digitalWrite(RowPin, LOW);

    for (int Col = 0; Col < 10; Col++) {

      keyPos    = Col + (Row * 10);
      outChar   = keyMap[keyPos];
      isKeyDown = !digitalRead(ColPinMap[Col]);

      if (millis() < lastDebounceTime[keyPos] + debounceDelay)
        continue;

      if (isKeyDown) special_keys();

      if (isKeyDown && !lastKeyState[keyPos]) {

        lastKeyState[keyPos] = true;
        lastDebounceTime[keyPos] = millis();

        #ifndef DEBUG
        if (outChar != NULL) BootKeyboard.press(outChar);
        #else
        Serial.print("col: ");
        Serial.print(Col);
        Serial.print(", row: ");
        Serial.print(Row);
        Serial.print(", Position: ");
        Serial.print(keyPos);
        Serial.print(", char:");
        Serial.print(outChar);
        Serial.println();
        #endif
      }

      if (!isKeyDown && lastKeyState[keyPos]) {

        lastKeyState[keyPos] = false;
        lastDebounceTime[keyPos] = millis();

        #ifndef DEBUG
        if (outChar != NULL)
        BootKeyboard.release(outChar);
        #else
        Serial.print("Released ");
        Serial.print(keyPos);
        Serial.println();
        #endif
      }
    }

    digitalWrite(RowPin, HIGH);
    delay(1);
    pinMode(RowPin, INPUT_PULLUP);
  }
}


// Uncomment to debug using serial monitor at 115200 baud
#define DEBUG

#include "HID-Project.h"

/* =======================================================================

 Commodore 64 USB HID for Pro Micro by Chris Garrett @makerhacks   
 
 Inspired by and based on code by DJ Sures (Synthiam.com)          


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
       

*/

#define KEYDELAY 30

int  thisKey;
bool isKeyDown;
int lastKeyState[80];
long lastDebounceTime[80];
int debounceDelay = 100;
int RowPinMap[8] = {9, 3, 4, 5, 6, 7, 8, 2};
int ColPinMap[10] = {10, 16, 14, 21, 18, 19, 20, 15, 1, 0};
char keymap[80] = 
{
// 1   2   3   4   5   6   7   8   9  10
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
};
  
char printchar;



void bootsetup() {

  Serial.begin(115200);
  BootKeyboard.begin();
  BootKeyboard.releaseAll();

// TOP ROW
keymap[71] = '`';
keymap[70] = '1';
keymap[73] = '2';
keymap[10] = '3';
keymap[13] = '4';
keymap[20] = '5';
keymap[23] = '6';
keymap[30] = '7';
keymap[33] = '8';
keymap[40] = '9';
keymap[43] = '0';
keymap[50] = '+';
keymap[53] = '-';
keymap[60] = '\\';
keymap[63] = KEY_HOME;
keymap[0] = KEY_BACKSPACE;


// SECOND ROW
keymap[72] = KEY_LEFT_CTRL;
keymap[76] = 'q';
keymap[11] = 'w';
keymap[16] = 'e';
keymap[21] = 'r';
keymap[26] = 't';
keymap[31] = 'y';
keymap[36] = 'u';
keymap[41] = 'i';
keymap[46] = 'o';
keymap[51] = 'p';
keymap[56] = '@';
keymap[61] = '*';
keymap[66] = '|';

// THIRD ROW

keymap[77] = KEY_ESC;
keymap[17] = KEY_LEFT_SHIFT;
keymap[12] = 'a';
keymap[15] = 's';
keymap[22] = 'd';
keymap[25] = 'f';
keymap[32] = 'g';
keymap[35] = 'h';
keymap[42] = 'j';
keymap[45] = 'k';
keymap[52] = 'l';
keymap[55] = ':';
keymap[62] = ';';
keymap[65] = '=';
keymap[1] = KEY_ENTER;

// BOTTOM ROW

keymap[75] = KEY_LEFT_WINDOWS;
keymap[17] = KEY_LEFT_SHIFT;
keymap[14] = 'z';
keymap[27] = 'x';
keymap[24] = 'c';
keymap[37] = 'v';
keymap[34] = 'b';
keymap[47] = 'n';
keymap[44] = 'm';
keymap[57] = ',';
keymap[54] = '.';
keymap[67] = '/';
keymap[64] = KEY_RIGHT_SHIFT;
keymap[7] = KEY_DOWN_ARROW;
keymap[2] = KEY_RIGHT_ARROW;

// SPACE AND F1-F8

keymap[74] = KEY_SPACE;
keymap[4] = KEY_F1;
keymap[5] = KEY_F3;
keymap[6] = KEY_F5;
keymap[3] = KEY_F7;

}  


void setup() {

  bootsetup();
  for (int i = 0; i < 80; i++) lastKeyState[i] = false;
  for (int Row = 0; Row < 8; Row++) pinMode(RowPinMap[Row], INPUT_PULLUP);
  for (int Col = 0; Col < 10; Col++) pinMode(ColPinMap[Col], INPUT_PULLUP);

}

bool ignoreKey(int keynum) {
  
        // Special Keys
          switch (keynum) 
          {
            
          case 74:
            return true;
            break;
   
          case 77:
            return true;
            break;
      
          case 1:
            return true;
            break;
    
          case 0:
            return true;
            break;
    
          case 63:        
            return true;
            break;

          case 2:
            return true;
            break;
            
          case 7:
            return true;
            break;
    
          }
        

         // Shifted / modifiers (eg. shift-2 = ")
         if ( shifted() ) {

            printchar = NULL;
      
            switch (keynum) {
              
            case 70:
              return true;
              break;
      
            case 73:
              return true;
              break;
            
            case 10:
              return true;
              break;
             
            case 13:
              return true;
              break;
             
            case 20:
              return true;
              break;
             
            case 23:
              return true;
              break;
             
            case 30:
              return true;
              break;
             
            case 33:
              printchar = '(';
              break;
       
            case 40:
              return true;
              break;
      
            case 55:
              printchar = '[';
              break;
      
            case 62:
              return true;
              break;
      
            case 57:
              return true;
              break;
      
            case 54:
              return true;
              break;
            
            case 67:
              return true;
              break;

       
          }

       }

       // Didn't match any of these keys/combos so return false
       return false;
}       


bool specialKeys(int keynum) {
  
        // Special Keys
          switch (keynum) 
          {
          case 74:
          
            BootKeyboard.press(KEY_SPACE);
            delay(debounceDelay);
            BootKeyboard.release(KEY_SPACE);
            Serial.println("SPACE");
            return true;
            break;
   
          case 77:
          
            BootKeyboard.press(KEY_ESC);
            delay(debounceDelay);
            BootKeyboard.release(KEY_ESC);
            Serial.println("RUNSTOP");
              return true;
            break;
      
          case 1:
          
            BootKeyboard.press(KEY_RETURN);      
            delay(debounceDelay);
            BootKeyboard.release(KEY_RETURN);      
            Serial.println("RETURN");           
              return true;
            break;
    
          case 0:

            if ( shifted() ) 
            {
              BootKeyboard.press(KEY_INSERT);
              delay(debounceDelay);
              BootKeyboard.release(KEY_INSERT);
              Serial.println("INS");           
            }
            else
            {
              BootKeyboard.press(KEY_BACKSPACE);
              delay(debounceDelay);
              BootKeyboard.release(KEY_BACKSPACE);
              Serial.println("DEL");    
            }
            return true;
            break;
    
          case 63:
          
            BootKeyboard.press(KEY_HOME);
            delay(debounceDelay);
            BootKeyboard.release(KEY_HOME);
            Serial.println("HOME");
              return true;
            break;

          case 2:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_LEFT);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_LEFT);  
              Serial.println("CURSOR LEFT");  
              return true;
            } 
            else 
            {
          
              BootKeyboard.press(KEY_RIGHT);  
              delay(debounceDelay);     
              BootKeyboard.release(KEY_RIGHT);                  
              Serial.println("CURSOR RIGHT");   
              return true;
            }
            break;
            
          case 7:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_UP);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_UP);  
              Serial.println("CURSOR UP");   
              return true;
             
            } 
            else 
            {
          
             BootKeyboard.press(KEY_DOWN);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_DOWN);  
              Serial.println("CURSOR DOWN");              
              return true;
            }      
            break;
    
          }
        

         // Shifted / modifiers (eg. shift-2 = ")
         if ( shifted() ) {

            printchar = NULL;
      
            switch (keynum) {
              
            case 70:
              printchar = '!';
              break;
      
            case 73:
              printchar = '"';
              break;
       
      
            case 10:
              printchar = '#';
              break;
       
      
            case 13:
              printchar = '$';
              break;
       
      
            case 20:
              printchar = '%';
              break;
       
      
            case 23:
              printchar = '&';
              break;
       
      
            case 30:
              printchar = '\'';
              break;
       
      
            case 33:
              printchar = '(';
              break;
       
      
            case 40:
              printchar = ')';
              break;
       
      
            case 55:
              printchar = '[';
              break;
       
      
            case 62:
              printchar = ']';
              break;
       
      
            case 57:
              printchar = '<';
              break;
       
      
            case 54:
              printchar = '>';
              break;
       
      
            case 67:
              printchar = '?';
              break;
       
          }

          if(printchar!=NULL) 
          {
            BootKeyboard.write(printchar);
              return true;
            
          }
       }

       // Didn't match any of these keys/combos so return false
       return false;
}       

 

bool shifted()
{

 if((lastKeyState[17] || lastKeyState[64]) && (thisKey!=17 && thisKey!=64)) 
  return true;
  else
  return false;
}

bool ckey()
{
  
 if(lastKeyState[75] && thisKey!=75) 
  return true;
  else
  return false;
}


bool ctrl()
{
  
 if(lastKeyState[72] && thisKey!=72) 
  return true;
  else
  return false;
  
}


void press(uint8_t key)
{

      if (shifted()) BootKeyboard.press(KEY_LEFT_SHIFT);
      if (ckey()) BootKeyboard.press(KEY_TAB);
      if (ctrl()) BootKeyboard.press(KEY_LEFT_CTRL);
      BootKeyboard.press(key);
  
}

void release(uint8_t key)
{

      BootKeyboard.release(key);

      if (shifted()) BootKeyboard.release(KEY_LEFT_SHIFT);
      if (ckey()) BootKeyboard.release(KEY_TAB);
      if (ctrl()) BootKeyboard.release(KEY_LEFT_CTRL);
}

void loop() {

  thisKey   = NULL;
  isKeyDown = NULL;

  for (int Row = 0; Row < 8; Row++) {

    int RowPin = RowPinMap[Row];
    pinMode(RowPin, OUTPUT);
    digitalWrite(RowPin, LOW);

    for (int Col = 0; Col < 10; Col++) {

      thisKey    = Col + (Row * 10);
      isKeyDown = !digitalRead(ColPinMap[Col]);

      // Non-blocking delay
      if (millis() < lastDebounceTime[thisKey] + debounceDelay) continue;

      // Is the key currently down and was before too?
      if (isKeyDown && lastKeyState[thisKey]) {
       
        // if(shifted()) Serial.print(" SHIFT ");

      }
      
      // Is the key currently down and wasn't before?
      if (isKeyDown && !lastKeyState[thisKey]) {

        // Toggle the key state
        lastKeyState[thisKey] = true;

        if (thisKey!=17 && thisKey!=64) {
          Serial.print(thisKey);
          Serial.print("('");
          Serial.write(keymap[thisKey]);               
          Serial.print("')");
          Serial.print("\n\r");
        }

        if(!specialKeys(thisKey)) press(keymap[thisKey]);
      }

      // The key is NOT down but WAS before
      if (!isKeyDown && lastKeyState[thisKey]) {

        // Toggle the key state
        lastKeyState[thisKey] = false;
        
        if(!ignoreKey(thisKey)) release(keymap[thisKey]);
      }
    }

    lastDebounceTime[thisKey] = millis();
    digitalWrite(RowPin, HIGH);
    delay(1);
    pinMode(RowPin, INPUT_PULLUP);
  }
}

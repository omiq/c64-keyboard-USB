
// Uncomment to debug using serial monitor at 115200 baud
// #define DEBUG

// Uncomment the correct target machine to use your C64 keyboard with
// For RetroPie try VICE
//#define U64
#define VICE
//#define MISTER
//#define BMC64


#include "HID-Project.h"

// RUNSTOP Vice uses KEY_ESC, u64 uses KEY_PAUSE
// CTRL vice uses KEY_TAB, u64 uses KEY_LEFT_WINDOWS
// MiSTer uses KEY_LEFT_CTRL as CTRL and C= is KEY_LEFT_ALT

#if defined(VICE) 
#define KEYSTOP KEY_ESC  
#define CKEY KEY_TAB
#define CTRLKEY KEY_LEFT_CTRL
#define LEFTARROW '_'
#define UPARROW '~'

#elif defined(MISTER)
#define KEYSTOP KEY_ESC  
#define CKEY KEY_LEFT_ALT
#define CTRLKEY KEY_LEFT_CTRL
#define LEFTARROW '_'
#define UPARROW KEY_F9

#elif defined(U64)
#define KEYSTOP KEY_PAUSE
#define CKEY KEY_LEFT_WINDOWS
#define CTRLKEY KEY_LEFT_CTRL
#define LEFTARROW '~'
#define UPARROW '|'

#elif defined(BMC64)
#define KEYSTOP KEY_ESC
#define CKEY KEY_LEFT_CTRL
#define CTRLKEY KEY_TAB
#define LEFTARROW '`'
#define UPARROW '^'

#else
#define KEYSTOP KEY_ESC
#define CKEY KEY_LEFT_WINDOWS
#define CTRLKEY KEY_LEFT_CTRL
#define LEFTARROW '_'
#define UPARROW '~'
#endif




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

void SerialPrintLine(String outstring)
{
  #ifdef DEBUG
  Serial.println(outstring);
  #endif
}

void SerialPrint(String outstring)
{
  #ifdef DEBUG
  Serial.print(outstring);
  #endif
}


void SerialWrite(char outstring)
{
  #ifdef DEBUG
  Serial.write(outstring);
  #endif
}


void bootsetup() {

  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  
  BootKeyboard.begin();
  BootKeyboard.releaseAll();

// TOP ROW
keymap[71] = LEFTARROW;   // vice uses _, u64 uses ~
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
keymap[71] = UPARROW;   

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

          case 0:
            return true;
            break;
      
          case 1:
            return true;
            break;

          case 2:
            return true;
            break;
            
          case 7:
            return true;
            break;

          case 63:        
            return true;
            break;

          case 66:        
            return true;
            break;

          case 71:
            return true;
            break;
               
          case 74:
            return true;
            break;
   
          case 77:
            return true;
            break;
  
          }
        

         // Shifted / modifiers (eg. shift-2 = ")
         if ( shifted() ) {

            printchar = NULL;
      
            switch (keynum) {
              
           
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
              return true;
              break;
       
            case 40:
              return true;
              break;
      
            case 54:
              return true;
              break;
              
            case 55:
              return true;
              break;
      
            case 57:
              return true;
              break;
              
            case 62:
              return true;
              break;
                  
            case 67:
              return true;
              break;
              
            case 70:
              return true;
              break;
      
            case 73:
              return true;
              break;

            case 75:
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
          case 66:
          
            BootKeyboard.press(UPARROW);
            delay(debounceDelay);
            BootKeyboard.release(UPARROW);
            SerialPrintLine("SPACE");
            return true;
            break;

          case 71:
          
            BootKeyboard.press(LEFTARROW);
            delay(debounceDelay);
            BootKeyboard.release(LEFTARROW);
            SerialPrintLine("SPACE");
            return true;
            break;
            
          case 74:
          
            BootKeyboard.press(KEY_SPACE);
            delay(debounceDelay);
            BootKeyboard.release(KEY_SPACE);
            SerialPrintLine("SPACE");
            return true;
            break;   

          case 75:

            if ( shifted() ) 
            {
              BootKeyboard.press(KEY_LEFT_SHIFT);
              BootKeyboard.press(CKEY);
              delay(debounceDelay);
              BootKeyboard.release(CKEY);
              BootKeyboard.release(KEY_LEFT_SHIFT);
              SerialPrintLine("CHANGE CASE");           
            }
   
          case 77:
                  
            BootKeyboard.press(KEYSTOP);  
            delay(debounceDelay);
            BootKeyboard.release(KEYSTOP);
            SerialPrintLine("RUNSTOP");
              return true;
            break;
      
          case 1:
          
            BootKeyboard.press(KEY_RETURN);      
            delay(debounceDelay);
            BootKeyboard.release(KEY_RETURN);      
            SerialPrintLine("RETURN");           
              return true;
            break;
    
          case 0:

            if ( shifted() ) 
            {
              BootKeyboard.press(KEY_LEFT_SHIFT);
              BootKeyboard.press(KEY_BACKSPACE);
              delay(debounceDelay);
              BootKeyboard.release(KEY_BACKSPACE);
              BootKeyboard.release(KEY_LEFT_SHIFT);
              SerialPrintLine("INS");           
            }
            else
            {
              BootKeyboard.press(KEY_BACKSPACE);
              delay(debounceDelay);
              BootKeyboard.release(KEY_BACKSPACE);
              SerialPrintLine("DEL");    
            }
            return true;
            break;
    
          case 63:

            if ( shifted() ) BootKeyboard.press(KEY_LEFT_SHIFT);
            BootKeyboard.press(KEY_HOME);
            delay(debounceDelay);
            BootKeyboard.release(KEY_HOME);
            if ( shifted() ) BootKeyboard.release(KEY_LEFT_SHIFT);
            SerialPrintLine("HOME");
              return true;
            break;

          case 2:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_LEFT);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_LEFT);  
              SerialPrintLine("CURSOR LEFT");  
              return true;
            } 
            else 
            {
          
              BootKeyboard.press(KEY_RIGHT);  
              delay(debounceDelay);     
              BootKeyboard.release(KEY_RIGHT);                  
              SerialPrintLine("CURSOR RIGHT");   
              return true;
            }
            break;
            
          case 7:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_UP);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_UP);  
              SerialPrintLine("CURSOR UP");   
              return true;
             
            } 
            else 
            {
          
             BootKeyboard.press(KEY_DOWN);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_DOWN);  
              SerialPrintLine("CURSOR DOWN");              
              return true;
            }      
            break;


          // F-KEYS
          case 4:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_F2);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_F2);  
              SerialPrintLine("F2");   
              return true;
             
            } 
            else 
            {
          
              BootKeyboard.press(KEY_F1);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_F1);  
              SerialPrintLine("F1");              
              return true;
            }      
            break;

          case 5:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_F4);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_F4);  
              SerialPrintLine("F4");   
              return true;
             
            } 
            else 
            {
          
              BootKeyboard.press(KEY_F3);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_F3);  
              SerialPrintLine("F3");              
              return true;
            }      
            break;


          case 6:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_F6);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_F6);  
              SerialPrintLine("F6");   
              return true;
             
            } 
            else 
            {
          
              BootKeyboard.press(KEY_F5);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_F5);  
              SerialPrintLine("F5");              
              return true;
            }      
            break;


          case 3:
            if ( shifted() ) 
            {
          
              BootKeyboard.press(KEY_F8);  
              delay(debounceDelay);
              BootKeyboard.release(KEY_F8);  
              SerialPrintLine("F8");   
              return true;
             
            } 
            else 
            {
                // C= F7 brings up the menu in BMC64
                if( ckey() )
                {
                  BootKeyboard.press(CKEY);                 
                  BootKeyboard.press(KEY_F7);  
                  delay(debounceDelay);
                  BootKeyboard.release(KEY_F7);  
                  BootKeyboard.press(CKEY);  
                  SerialPrintLine("C= F7");              
                  return true;
                }
                else
                {
                  BootKeyboard.press(KEY_F7);  
                  delay(debounceDelay);
                  BootKeyboard.release(KEY_F7);  
                  SerialPrintLine("F7");              
                  return true;
                }
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
            BootKeyboard.press(printchar);  
            delay(debounceDelay);
            BootKeyboard.release(printchar);  
            SerialWrite(printchar);
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
      if (ckey()) BootKeyboard.press(CKEY);   
      if (ctrl()) BootKeyboard.press(CTRLKEY);
      BootKeyboard.press(key);
  
}

void release(uint8_t key)
{

      BootKeyboard.release(key);

      if (shifted()) BootKeyboard.release(KEY_LEFT_SHIFT);
      if (ckey()) BootKeyboard.release(CKEY);
      if (ctrl()) BootKeyboard.release(CTRLKEY);
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
       
        // if(shifted()) SerialPrint(" SHIFT ");

      }
      
      // Is the key currently down and wasn't before?
      if (isKeyDown && !lastKeyState[thisKey]) {

        // Toggle the key state
        lastKeyState[thisKey] = true;

        if (thisKey!=17 && thisKey!=64) {
          SerialPrint(String(thisKey));
          SerialPrint("('");
          SerialWrite(keymap[thisKey]);               
          SerialPrint("')");
          SerialPrint("\n\r");
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

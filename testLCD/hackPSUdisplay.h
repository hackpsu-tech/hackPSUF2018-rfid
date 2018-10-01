#pragma once

#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

//#include "hackPSUconfig.h"

namespace hackPSU {

  typedef enum {PROD, DEV, HEADLESS} Mode_e;
  typedef enum {CHECK, UP, DOWN, CLEAR, BACK, LOCK, SCROLL} Custom_char;
  class Display{
    private:
      Mode_e mode;
      LiquidCrystal_I2C* lcd;
      String data[2];
      int row;
      bool scrolled;

    public:
      Display(Mode_e mode);
      ~Display();

      /**
       * Description:
       *    Prints msg starting at the current location of the cursor
       */
      void print(String msg);

      /**
       * Description:
       *    Prints the character at the current location of the cursor
       */
      void print(char msg);

      /**
       * Description:
       *    Clears the specified row then prints msg on that row leaving the cursor at the end of the string.
       *    If msg is currently displayed on the specified row, it will not be displayed.
       */
      void print(String msg, int row);

      /**
       * Description:
       *    Clears all contents of the LCD and returns the cursor to (0, 0)
       */

      void print(Custom_char symbol);

      /**
       * Description:
       *    Prints the custom character at the current location of the cursor
       */

      void print(char msg, Custom_char symbol);
       
      void scroll();

      /**
       * Description:
       *    Scrolls contents of the LCD
       */
      void clear();

      /**
       * Description:
       *    Clears the requested row then sets the cursor to the beginning of that row
       */
      void clear(int row);

      /**
       * Description:
       *    Clears the last <num> characters and moved the cursor back to the new end of the string
       */
      void backspace(int num);
  };

}

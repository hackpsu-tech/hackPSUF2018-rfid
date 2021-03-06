#pragma once

#include <Arduino.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <config.h>

namespace hackPSU {

  typedef enum {PROD, DEV, HEADLESS} Mode_e;
  typedef enum {CHECK_C, UP_C, DOWN_C, CLEAR_C, BACK_C, LOCK_C, SCROLL_C, NONE_C} Custom_char;
  struct Menu_item{
    char key;
    Custom_char symbol;
  };
  class Display{
    private:
      Mode_e mode;
      LiquidCrystal_I2C* lcd;
      String data[2];
      Menu_item menu[4];
      int row;
      bool scrolled;
      
      void print(Custom_char symbol);
      void print(Menu_item control);

    public:
      Display(Mode_e mode);
      ~Display();

      void toggleDisplay();

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
       *    Takes 4 sets of char's and Custom_char's to display the menu across a 16 column LCD display.  If the Custom_char is NONE_C, 
       *    then the character doesn't matter (can use \0 or ' ' etc.).  If any one of these values change from the previous menu state, 
       *    the entire menu will be reprinted with 4 spaces in any position of NONE_C.
       */

      void print(char key1, Custom_char symbol1, char key2, Custom_char symbol2, char key3, Custom_char symbol3, char key4, Custom_char symbol4);
      
      /**
       * Description:
       *    Scrolls contents of the LCD to the end then returns the display to what it was before.
       */ 
      #ifdef LCD_EN
      void scroll();
      #endif

       /**
       * Description:
       *    Clears all contents of the LCD and returns the cursor to (0, 0)
       */
      #ifdef LCD_EN
      void clear();
      #endif

      /**
       * Description:
       *    Clears the requested row then sets the cursor to the beginning of that row
       */
      #ifdef LCD_EN
      void clear(int row);
      #endif

      /**
       * Description:
       *    Clears the last <num> characters and moved the cursor back to the new end of the string
       */
      #ifdef LCD_EN
      void backspace(int num);
      #endif
  };
}

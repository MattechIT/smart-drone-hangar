#ifndef __LCD__
#define __LCD__

#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

class Lcd {
public:
    Lcd(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
    
    void init();

    void print(String text);
    
    void print(String line1, String line2);
    
    void clear();

private:
    LiquidCrystal_I2C lcd;
};

#endif
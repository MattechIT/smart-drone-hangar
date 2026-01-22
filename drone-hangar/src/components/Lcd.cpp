#include "Lcd.h"

Lcd::Lcd(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : 
    lcd(lcd_Addr, lcd_cols, lcd_rows) {
}

void Lcd::init(){
    this->lcd.init();
    this->lcd.backlight();
}

void Lcd::print(String text){
    this->lcd.clear();
    this->lcd.setCursor(0,0);
    this->lcd.print(text);
}

void Lcd::print(String line1, String line2){
    this->lcd.clear();
    this->lcd.setCursor(0,0);
    this->lcd.print(line1);
    this->lcd.setCursor(0,1);
    this->lcd.print(line2);
}

void Lcd::clear(){
    this->lcd.clear();
}
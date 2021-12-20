#ifndef __U8X8_DISPLAY_H__

#define __U8X8_DISPLAY_H__


#include <basic_menu.h>

#ifdef EN_M_U8X8

class u8x8_display:public text_display{
public:
    U8X8 *display;
    
    u8x8_display(U8X8 *disp){
        display = disp;
        get_display_data();
    };


    void get_display_data();
    void clear_row(uint8_t row,uint8_t col, uint8_t w=0,uint8_t rev=0);
    void print(uint8_t row,uint8_t col,const char *txt,uint8_t rev=0);
    void clear_area(uint8_t row,uint8_t col, uint8_t w,uint8_t h){};
    void clear_display(){display->clearDisplay();};
    void refresh(){};
};

#endif

#endif

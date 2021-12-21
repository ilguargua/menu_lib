#ifndef __SERIAL_DISPLAY_H__

#define __SERIAL_DISPLAY_H__

#include <basic_menu.h>

#ifdef EN_M_SERIAL


class serial_display:public text_display{
public:
    Stream *display;
    
    serial_display(Stream *disp){
        display = disp;
        rows = 255;
        cols = 80;
    };
    //curses_display(uint8_t r,uint8_t c,uint8_t y,uint8_t x);

    void get_display_data(){rows=255;cols=80;};
    void clear_row(uint8_t row,uint8_t col, uint8_t w=0,uint8_t rev=0){};
    void print(uint8_t row,uint8_t col,const char *txt,uint8_t rev=0){
        if(rev > 0) display->print("> ");
        else display->print("  ");
        display->println(txt);
    };
    void clear_area(uint8_t row,uint8_t col, uint8_t w,uint8_t h){};
    void clear_display(){};
    void refresh(){};
};

#endif

#endif

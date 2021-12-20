#ifndef __LQ_DISPLAY_H__

#define __LQ_DISPLAY_H__


#include <basic_menu.h>

#ifdef EN_M_LQ

class lq_display:public text_display{
public:
    LiquidCrystal *display;
    
    lq_display(LiquidCrystal *disp,uint8_t c,uint8_t r){
        display = disp;
        cols = c;
        rows = r;
        //get_display_data();
    };


    //void get_display_data();
    void clear_row(uint8_t row,uint8_t col, uint8_t w=0,uint8_t rev=0);
    void print(uint8_t row,uint8_t col,const char *txt,uint8_t rev=0);
    void clear_area(uint8_t row,uint8_t col, uint8_t w,uint8_t h){};
    void clear_display(){display->clear();};
    void refresh(){};
};

#endif

#endif

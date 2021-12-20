#include <lq_display.h>

#ifdef EN_M_LQ


void lq_display::print(uint8_t row,uint8_t col,const char *txt,uint8_t rev){
    char buf[20];  //max liquid crystal cols
    memset(buf,0,20);
    display->setCursor(col,row);
    display->noCursor();
    if(rev > 0 ){
        if(rev == 1) display->print(">");
        else {
            display->setCursor(col+1,row);
            //display->cursor();
        }
    }
    else display->print(" ");
    //strncpy(buf,txt,cols-1);
    strncpy(buf,txt,cols-col-1);
    display->print(buf);
    if(rev == 2){
        display->setCursor(col+1,row);
        display->cursor();
    }
}


void lq_display::clear_row(uint8_t row,uint8_t col, uint8_t w,uint8_t rev){
    
    if(w == 0) w = cols - col ; //default to end of line
    memset(conv_buf,0,MENU_BUF_LEN);
    memset(conv_buf,' ',min(w,MENU_BUF_LEN-1));
    //display->setInverseFont(0);
    display->setCursor(col,row);
    display->print(conv_buf);
}

#endif

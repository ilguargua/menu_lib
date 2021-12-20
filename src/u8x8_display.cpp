
#include <u8x8_display.h>

#ifdef EN_M_U8X8

void u8x8_display::get_display_data(){
    rows = display->getRows();
    cols = display->getCols();
    if(rows == 0){
        display->begin();
        display->setFont(u8x8_font_chroma48medium8_r);
        rows = display->getRows();
        cols = display->getCols();
    }
}

void u8x8_display::print(uint8_t row,uint8_t col,const char *txt,uint8_t rev){
    if(rev > 0 ) display->setInverseFont(1);
    else display->setInverseFont(0);
    display->drawString(col,row,txt);
}


void u8x8_display::clear_row(uint8_t row,uint8_t col, uint8_t w,uint8_t rev){
    
    if(w == 0) w = cols - col ; //default to end of line
    memset(conv_buf,0,MENU_BUF_LEN);
    memset(conv_buf,' ',w);
    display->setInverseFont(0);
    display->drawString(col,row,conv_buf);
}

#endif

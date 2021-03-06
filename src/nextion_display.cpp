#include <nextion_display.h>

#ifdef EN_M_NEXTION


nextion_display::nextion_display(){
    ser = nullptr;
    rows = 0;
    cols = 0;
    row_h = 0;
    col_l = 0;
    font = 0;
    d_cols = 0;
    d_rows = 0;
    options = 0;
}

nextion_display::nextion_display(Stream *disp,uint8_t r,uint8_t c,uint8_t f_ndx,uint8_t f_h,uint8_t f_l){
    init(disp,r,c,f_ndx,f_h,f_l);
}

void nextion_display::init(Stream *disp,uint8_t r,uint8_t c,uint8_t f_ndx,uint8_t f_h,uint8_t f_w){
    ser = disp;
    rows = r;
    cols = c;
    row_h = f_h+2;
    col_l = f_w;//+2;
    font = f_ndx;
    d_cols = get_nextion_data('w')/f_w;
    d_rows = get_nextion_data('h')/f_h;
    
    
    if(rows > d_rows) rows = d_rows;
    if(cols > d_cols) cols = d_cols;
    if(d_rows > rows){
        menu_y = (((d_rows-rows)/2)*f_h)+1;
    }
    else menu_y = 1;
    if(d_cols > cols){
        menu_x = (((d_cols-cols)/2)*f_w)+1;
    }
    else menu_x = 1;
    options = 0;

};

void nextion_display::set_options(uint8_t opts,uint8_t set){
    if(set > 0) options |= opts;
    else //options ^= opts;    
    {
        if(options & opts) options -= opts;
    }
}


void nextion_display::print(uint8_t row,uint8_t col,const char *txt,uint8_t rev){
    if(options & NXT_PROP_FONT) print_prop(row,col,txt,rev);
    else print_mono(row,col,txt,rev);
}


//Normal printing, works well with mono-spaced fonts
void nextion_display::print_mono(uint8_t row,uint8_t col,const char *txt,uint8_t rev){
    if(ser == nullptr) return;
    uint16_t fg = rev > 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    uint16_t bg = rev == 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,XSTR_FMT,
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,XSTR_FMT,
#endif             
            menu_x+(col*col_l)+2,
            menu_y+(row*row_h)+2,
            strlen(txt)*col_l,
            row_h-2,
            font,
            fg,
            bg,
            txt,
            NXT_MSG_END
            );
    ser->print(nxt_buf);
}


//Proportional font printing
//We print each char in is own box, so recrating mono-spaced
//Ugly and slow
void nextion_display::print_prop(uint8_t row,uint8_t col,const char *txt,uint8_t rev){
    if(ser == nullptr) return;
    uint16_t fg = rev > 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    uint16_t bg = rev == 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    uint8_t l = strlen(txt);
    char b[2] = {0,0};
    uint16_t msg_size = 0;
    memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,COM_STOP,NXT_MSG_END);
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,COM_STOP,NXT_MSG_END);
#endif             
    ser->print(nxt_buf);
    
    for(uint8_t i=0;i<l;i++){
        b[0] = txt[i];
        memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
        snprintf_P(nxt_buf,NXT_BUF_SIZE-1,XSTR_FMT,
#else
        snprintf(nxt_buf,NXT_BUF_SIZE-1,XSTR_FMT,
                            #endif             
                            menu_x+2+(i*col_l)+(col *col_l),
                            menu_y+(row*row_h)+2,
                            col_l,
                            row_h-2,
                            font,
                            fg,
                            bg,
                            b,
                            NXT_MSG_END
        );
        if((msg_size+strlen(nxt_buf)+20) > NXT_COM_SIZE){
            ser->print("com_star");
            ser->write(0xFF);
            ser->write(0xFF);
            ser->write(0xFF);
            delay(50);
            ser->print("com_stop");
            ser->write(0xFF);
            ser->write(0xFF);
            ser->write(0xFF);
            msg_size = 0;
        } 
        else msg_size += strlen(nxt_buf);
        ser->print(nxt_buf);
    }
    memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,COM_START,NXT_MSG_END);
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,COM_START,NXT_MSG_END);
#endif             
    ser->print(nxt_buf);
    //wait for the display to process data and empty com buffer
    delay(10);
}



void nextion_display::clear_row(uint8_t row,uint8_t col, uint8_t w,uint8_t rev){
    if(ser == nullptr) return;
    uint16_t fc = rev == 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    if(w == 0) w = cols - col ; //default to end of line
    memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)        
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,FILL_FMT,
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,FILL_FMT,
#endif             
            menu_x+(col*col_l)+2,
            menu_y+(row*row_h)+2,
            (w*col_l)-4,
            row_h-2,
            fc,
            NXT_MSG_END
            );
    ser->print(nxt_buf);
}


void nextion_display::clear_display(){
    if(ser == nullptr) return;
    if(options & NXT_LOCK){
        memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)            
        snprintf_P(nxt_buf,NXT_BUF_SIZE-1,TOFF_FMT,NXT_MSG_END);
#else
        snprintf(nxt_buf,NXT_BUF_SIZE-1,TOFF_FMT,NXT_MSG_END);
#endif    
        ser->print(nxt_buf);
    }
    memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,FILL_FMT,
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,FILL_FMT,
#endif             
            menu_x - 4,
            menu_y - 4,
            (cols * col_l)+8,
            (rows * row_h)+8,
            NXT_BG_COLOR,
            NXT_MSG_END
            );
    ser->print(nxt_buf);
    memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,DRAW_FMT,
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,DRAW_FMT,
#endif             
            menu_x-2 ,
            menu_y-2 ,
            menu_x+(cols * col_l)+2,
            menu_y+(rows * row_h)+2,
            NXT_FG_COLOR,
            NXT_MSG_END
            );
   ser->print(nxt_buf);
}


uint16_t nextion_display::get_nextion_data(char d){
    if(ser == nullptr) return 0;
    uint8_t     cnt = 0;
    char        b = 0;
    memset(nxt_buf,0,NXT_BUF_SIZE);
    //getting display data by asking page width and height.
    //Empty serial buffer in case there is some previous data
    //WARNING thi may lost some data your progr is expecting
    while(ser->available()) ser->read();
#if defined(ARDUINO_ARCH_AVR)
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,GET_FMT,d,NXT_MSG_END);
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,GET_FMT,d,NXT_MSG_END);
#endif
    ser->print(nxt_buf);
    memset(nxt_buf,0,NXT_BUF_SIZE);
    delay(5);
    while(ser->available()){
        b = ser->read();
        if(b != -1 and cnt < NXT_BUF_SIZE-1){
            nxt_buf[cnt]=b;
            cnt++;
        }
    }
    if(nxt_buf[0] == 0x71) return (uint16_t)((uint8_t)nxt_buf[1]+((uint8_t)nxt_buf[2]*256));
    return 0;
}

void nextion_display::restore_display(){
    if(ser == nullptr) return;
    memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(nxt_buf,NXT_BUF_SIZE-1,REF_FMT,NXT_MSG_END);
#else
    snprintf(nxt_buf,NXT_BUF_SIZE-1,REF_FMT,NXT_MSG_END);
#endif    
    ser->print(nxt_buf);
    if(options & NXT_LOCK){
        memset(nxt_buf,0,NXT_BUF_SIZE);
#if defined(ARDUINO_ARCH_AVR)    
        snprintf_P(nxt_buf,NXT_BUF_SIZE-1,TON_FMT,NXT_MSG_END);
#else
        snprintf(nxt_buf,NXT_BUF_SIZE-1,TON_FMT,NXT_MSG_END);
#endif    
        ser->print(nxt_buf);
    }
}

#endif

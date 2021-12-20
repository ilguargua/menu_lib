#include <nextion_display.h>

#ifdef EN_M_NEXTION



nextion_display::nextion_display(Stream *disp,uint8_t r,uint8_t c,uint8_t f_ndx,uint8_t f_h,uint8_t f_l){
    ser = disp;
    rows = r;
    cols = c;
    row_h = f_h;
    col_l = f_l;
    font = f_ndx;
    d_cols = get_nextion_data('w')/f_l;
    d_rows = get_nextion_data('h')/f_h;
    if(rows > d_rows) rows = d_rows;
    if(cols > d_cols) cols = d_cols;
    if(d_rows > rows){
        menu_y = (((d_rows-rows)/2)*f_h)+1;
    }
    else menu_y = 1;
    if(d_cols > cols){
        menu_x = (((d_cols-cols)/2)*f_l)+1;
    }
    else menu_x = 1;
    //memset(conv_buf,0,MENU_BUF_LEN);

        //get_display_data();
};

void nextion_display::print(uint8_t row,uint8_t col,const char *txt,uint8_t rev){
    uint16_t fg = rev > 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    uint16_t bg = rev == 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    memset(conv_buf,0,MENU_BUF_LEN);
    snprinf(conv_buf,MENU_BUF_LEN-1,XSTR_FMT,
            menu_x+(row*col_l),
            menu_y+(col*row_h),
            strlen(txt)*col_l,
            row_h,
            font,
            fg,
            bg,
            txt,
            NXT_MSG_END
            );
    ser.print(conv_buf);
}


void nextion_display::clear_row(uint8_t row,uint8_t col, uint8_t w,uint8_t rev){
    uint16_t fc = rev == 0 ? NXT_BG_COLOR : NXT_FG_COLOR;
    if(w == 0) w = cols - col ; //default to end of line
    memset(conv_buf,0,MENU_BUF_LEN);
    snprinf(conv_buf,MENU_BUF_LEN-1,FILL_FMT,
            menu_x+(row*col_l),
            menu_y+(col*row_h),
            w*col_l,
            row_h,
            fc
            );
}


void nextion_display::clear__display(){
    snprinf(conv_buf,MENU_BUF_LEN-1,FILL_FMT,
            menu_x - 1,
            menu_y - 1,
            cols * f_l,
            rows * f_h,
            NXT_BG_COLOR,
            NXT_MSG_END
            );
    ser.print(conv_buf);
    memset(conv_buf,0,MENU_BUF_LEN);
    snprinf(conv_buf,MENU_BUF_LEN-1,DRAW_FMT,
            menu_x ,
            menu_y ,
            menu_x+(cols * f_l),
            menu_y+(rows * f_h),
            NXT_FG_COLOR,
            NXT_MSG_END
            );
   ser.print(conv_buf);
}


uint16_t nextion_display::get_nextion_data(char d){
    uint8_t     cnt = 0;
    char        b = 0;
    memset(conv_buf,0,MENU_BUF_LEN);
    //getting display data by asking page width and height
    //empty serial buffer in case there is some previous data
    //WARNING thi may lost some data your progr is expecting
    while(ser.available()) ser.read();
    snprinf(conv_buf,MENU_BUF_LEN-1,GET_FMT,d,NXT_MSG_END);
    ser.print(conv_buf);
    memset(conv_buf,0,MENU_BUF_LEN);
    delay(5);
    while(ser.available()){
        b = ser.read();
        if(b != -1 and cnt < MENU_BUF_LEN-1){
            conv_buf[cnt]=b;
            cnt++;
        }
    }
    if(strlen(conv_buf) == 8 and conv_buf[0] == 0x71)return conv_buf[1]+(conv_buf[2]*256);
    return 0;
}

void nextion_display::restore_display(){
    memset(conv_buf,0,MENU_BUF_LEN);
    snprinf(conv_buf,MENU_BUF_LEN-1,REF_FMT,NXT_MSG_END);
    ser.print(conv_buf);
}

#endif

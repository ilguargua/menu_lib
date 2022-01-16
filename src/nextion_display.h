#ifndef __NEXTION_DISPLAY_H__

#define __NEXTION_DISPLAY_H__


#include <basic_menu.h>

#ifdef EN_M_NEXTION

typedef enum {
    NXT_BLACK =  0,
    NXT_BLUE =   31,
    NXT_GREEN =  2016,
    NXT_GRAY =   33840,
    NXT_BROWN =  48192,
    NXT_RED =    63488,
    NXT_YELLOW = 65504,
    NXT_WHITE =  65535
} nextionColor_t; 

#define NXT_MSG_END         0xFF,0xFF,0xFF 
#define NXT_FG_COLOR        NXT_RED
#define NXT_BG_COLOR        NXT_BLUE

//Nextion need a bigger buffer size 
//so this intf define his own buffer
#define NXT_BUF_SIZE        100
#define NXT_COM_SIZE        1024

#if defined(ARDUINO_ARCH_AVR)

#define FILL_FMT            PSTR("fill %u,%u,%u,%u,%u%c%c%c")
#define XSTR_FMT            PSTR("xstr %u,%u,%u,%u,%u,%u,%u,0,1,1,\"%s\"%c%c%c")
#define DRAW_FMT            PSTR("draw %u,%u,%u,%u,%u%c%c%c")
#define REF_FMT             PSTR("ref 0%c%c%c")
#define GET_FMT             PSTR("get p[0].b[0].%c%c%c%c")
#define TOFF_FMT            PSTR("tsw 255,0%c%c%c")
#define TON_FMT             PSTR("tsw 255,1%c%c%c")
#define COM_STOP            PSTR("com_stop%c%c%c")
#define COM_START           PSTR("com_star%c%c%c")


#else

#define FILL_FMT            "fill %u,%u,%u,%u,%u%c%c%c"
#define XSTR_FMT            "xstr %u,%u,%u,%u,%u,%u,%u,0,1,1,\"%s\"%c%c%c"
#define DRAW_FMT            "draw %u,%u,%u,%u,%u%c%c%c"
#define REF_FMT             "ref 0%c%c%c"
#define GET_FMT             "get p[0].b[0].%c%c%c%c"
#define TOFF_FMT            "tsw 255,0%c%c%c"
#define TON_FMT             "tsw 255,1%c%c%c"
#define COM_STOP            "com_stop%c%c%c"
#define COM_START           "com_star%c%c%c"

#endif

typedef enum{
    NXT_LOCK        = 1,    //lock display control while in use, restore_display() will unlock
    NXT_PROP_FONT   = 2,    //use proportional font correction, useful for edit but slow
    NXT_OPT_3       = 4,
    NXT_OPT_4       = 8
} nxt_opts_t;

class nextion_display:public text_display{
public:
    Stream *ser;
    
    uint16_t d_rows;
    uint16_t d_cols;
    uint8_t menu_x; //upper left menu window corner
    uint8_t menu_y;
    uint8_t row_h;
    uint8_t col_l;
    uint8_t font;
    uint8_t options;
    char    nxt_buf[NXT_BUF_SIZE];
    
    nextion_display();
    nextion_display(Stream *disp,uint8_t r,uint8_t c,uint8_t f_ndx,uint8_t f_h,uint8_t f_l);

    void init(Stream *disp,uint8_t r,uint8_t c,uint8_t f_ndx,uint8_t f_h,uint8_t f_l);
    //void get_display_data(){};
    void clear_row(uint8_t row,uint8_t col, uint8_t w=0,uint8_t rev=0);
    void print(uint8_t row,uint8_t col,const char *txt,uint8_t rev=0);
    void print_mono(uint8_t row,uint8_t col,const char *txt,uint8_t rev=0);
    void print_prop(uint8_t row,uint8_t col,const char *txt,uint8_t rev=0);
    void clear_area(uint8_t row,uint8_t col, uint8_t w,uint8_t h){};
    void clear_display();
    void restore_display();
    void refresh(){};
    void set_options(uint8_t opts,uint8_t set=1);
private:
    
    uint16_t get_nextion_data(char d);
};

#endif

#endif

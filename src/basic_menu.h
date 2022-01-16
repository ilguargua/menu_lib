/*
 * menu_lib
 * An Arduino library for menus & editing variables
 * 
 * by ilguargua@gmail.com - 2021
 * 
 */



#ifndef __BASIC_MENU_H__

#define __BASIC_MENU_H__

//enable/disable interfaces

//#define EN_M_CURSES
//#define EN_M_U8G2
//#define EN_M_U8X8   1
//#define EN_GFX
//#define EN_M_LQ
#define EN_M_SERIAL
#define EN_M_NEXTION

#ifdef EN_M_CURSES
#include <curses.h>
#endif
#ifdef EN_M_U8G2
#include <U8g2lib.h>
#endif
#ifdef EN_M_U8X8
#include <U8x8lib.h>
#endif
#ifdef EN_M_LQ
#include <LiquidCrystal.h>
#endif


//This is the len of memory buffer used by text_display and edit_item classes
//Should be long at least as the numb. of char of display+1
#define MENU_BUF_LEN 21




#ifdef ARDUINO
#include <Arduino.h>
#else

#include <iostream> 
#include <cstring>
#include <cstdlib>


#endif

using namespace std;


#ifndef constrain
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif

#ifndef itoa
#define USE_SPRINTF
#endif

typedef enum{
    M_NONE,     //0 - no output
    M_CURSES,   //1 - curses interface
    M_U8G2,     //2 - U8G2 intf.
    M_U8X8,     //3 - U8X8
    M_GFX,      //4 - Adafruit GFX
    M_LQ,       //5 - Liquid Crystal 
    M_SERIAL,   //6 - Serial
    M_NEXTION   //7 - Nextion
} m_intf_t;

typedef struct{
    const char      *base;
    uint8_t         cnt;
    uint8_t         len;
    const char *operator[](int8_t i){
        i=constrain(i,0,cnt-1);
        if(base != nullptr) return base + (len * i);
        else return nullptr;
    };
} ch_arr_t;


typedef enum{
    M_NEW_DEV     = 1,
    M_NO_ROLLOVER = 2,
    M_DRAW_CLEAR  = 4,
    M_PRINT_CLEAR = 8,
    M_OPT5        = 16,
    M_OPT6        = 32,
    M_OPT7        = 64,
    M_OPT8        = 128
} b_menu_opt_t;


class text_display{
public:
    
    uint8_t rows;
    uint8_t cols;
    char    conv_buf[MENU_BUF_LEN];
    
    
    text_display(){
        rows = 0;
        cols = 0;
        memset(conv_buf,0,MENU_BUF_LEN);
    };

    virtual void get_display_data(){};
    //bool chk_data(){return rows > 0 && cols >0; };
    virtual void clear_row(uint8_t row,uint8_t col, uint8_t w=0,uint8_t rev=0){};
    virtual void clear_area(uint8_t row,uint8_t col, uint8_t w,uint8_t h){};
    virtual void clear_display(){};
    virtual void refresh(){};
    virtual void print(uint8_t row,uint8_t col,const char *txt,uint8_t rev=0){};
};


class basic_menu{
public:
    
    text_display    *device;
    uint8_t         cur_item;
    uint8_t         start_item;
    ch_arr_t        items;
    uint8_t         disp_rows;
    uint8_t         options;
    
#if defined(ARDUINO_ARCH_AVR) 
    //avr use pgm strings, so we need a buffer to copy them to ram
    char avr_buf[MENU_BUF_LEN];
    const __FlashStringHelper *title;
#else
    const char      *title;
#endif    
    
    
    basic_menu();
    //basic_menu(uint8_t dev_type,void *dev); //create text_display instance with new
    basic_menu(const char *base,uint8_t cnt,uint8_t item_l,uint8_t d_rows); //device = nullptr
    basic_menu(text_display *dev,const char *base,uint8_t cnt,uint8_t item_l,uint8_t d_rows = 0);//use dev
    virtual const char      *get_row(uint8_t row);
    void            set_items(const char *base,uint8_t cnt,uint8_t item_l);
    void            set_device(text_display *dev);
    //void            set_device(uint8_t dev_type,void *dev);
    
    uint8_t         get_cur_row();
    void            move_next();
    void            move_prev();
#if defined(ARDUINO_ARCH_AVR)    
    void            set_title(const __FlashStringHelper *tit);
#else
    void            set_title(const char *tit);
#endif    
    
    void            set_options(uint8_t opts, uint8_t set=1); //0 to unset
    void            draw_menu();
    void            set_rows(uint8_t r){disp_rows=r;};
    //text_display    *new_device(uint8_t dev_type,void *dev);
    //~basic_menu(){if(options & M_NEW_DEV) delete device;};
};




#endif









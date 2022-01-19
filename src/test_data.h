#include <edit_menu.h>
#ifndef __TEST_DATA_H__

#define __TEST_DATA_H__


#define DEF_EDIT_ITEMS  1


//Input methods
//uncomment only one of the following
//#define MNU_USE_ENCODER     1
//#define MNU_USE_PB          1
#define MNU_USE_SERIAL      1


//If using MNU_USE_PB how many push buttons to use
//uncomment only one
#define MNU_3_BTNS          3   //up down ok
//#define MNU_4_BTNS          4   //up down ok back (right)
//#define MNU_5_BTNS          5   //up down ok left right

//inputs defs

#define ENC_CH_A    2
#define ENC_CH_B    3
#define ENC_PB      4

#define MNU_PB1     5   //up
#define MNU_PB2     6   //down
#define MNU_PB3     7   //ok
#define MNU_PB4     8   //left
#define MNU_PB5     9   //right

#define MNU_PB_UP   MNU_PB1
#define MNU_PB_DN   MNU_PB2
#define MNU_PB_OK   MNU_PB3
#define MNU_PB_LE   MNU_PB4
#define MNU_PB_RI   MNU_PB5

typedef enum{
    M_PB_NONE,
    M_PB_UP,
    M_PB_DN,
    M_PB_OK,
    M_PB_LE,
    M_PB_RI
} m_pb_stat_t;

#define MNU_INP_LEN     5

#define PB_ACTIVE       LOW
#define PB_NOT_ACTIVE   HIGH

uint8_t           pb_prev_st[MNU_INP_LEN] = {PB_NOT_ACTIVE,PB_NOT_ACTIVE,PB_NOT_ACTIVE,PB_NOT_ACTIVE,PB_NOT_ACTIVE};
const uint8_t     btns[MNU_INP_LEN] = {MNU_PB_UP,MNU_PB_DN,MNU_PB_OK,MNU_PB_RI,MNU_PB_LE};
const char        ser_cod[MNU_INP_LEN] = {'u','d','o','l','r'};


uint8_t ck_btns(){
    uint8_t res = M_PB_NONE;
    for(uint8_t i =0;i<MNU_INP_LEN;i++){
        res = pb_check(i);
        if(res != M_PB_NONE) return res;
    }
    return res;
}

uint8_t pb_check(uint8_t btn_ndx){
    if(digitalRead(btns[btn_ndx]) == PB_ACTIVE){
        if(pb_prev_st[btn_ndx] == PB_NOT_ACTIVE) pb_prev_st[btn_ndx] = PB_ACTIVE;
        return M_PB_NONE;
    }
    else{
        if(pb_prev_st[btn_ndx] == PB_ACTIVE){
            pb_prev_st[btn_ndx] = PB_NOT_ACTIVE;
            return btn_ndx+1;
        }
    }
    return M_PB_NONE;
}


uint8_t serial_check(Stream &serial){
    char c = -1;
    if(serial.available()){
        c = serial.read();
        for(uint8_t i=0;i<MNU_INP_LEN;i++){
            if(ser_cod[i] == c){
                while(serial.available()) serial.read();
                return i+1;
            }
        }
    }
    while(serial.available()) serial.read();
    return M_PB_NONE;
}

const uint8_t mm_items_cnt = 6;
const uint8_t mm_items_len =15;
#if defined(ARDUINO_ARCH_AVR)
const PROGMEM char mm_items[][mm_items_len+1] = {
#else
const char mm_items[][mm_items_len+1]={// 1234567890123456
#endif    
                                         "Item 1",
                                         "Item two",
                                         "Third item",
                                         "Item 4",
                                         "Item five",
                                         "Sixth item"};


                                         
                                         

const uint8_t s1_cnt = 6;
const uint8_t s1_len =15;
#if defined(ARDUINO_ARCH_AVR)
const PROGMEM char s1_items[][s1_len+1]={// 1234567890123456
#else
const char s1_items[][s1_len+1]={//  1234567890123456
#endif
                                    "sub1-Item 1",
                                    "sub1-Item two",
                                    "sub1-Third item",
                                    "sub1-Item 4",
                                    "sub1-Item five",
                                    "sub1-back <-"};


const uint8_t s2_cnt = 6;
const uint8_t s2_len =15;
#if defined(ARDUINO_ARCH_AVR)
const PROGMEM char s2_items[][s2_len+1]={// 1234567890123456
#else
const char s2_items[][s2_len+1]={//  1234567890123456
#endif
                                    "sub2-Item 1",
                                    "sub2-Item two",
                                    "sub2-Third item",
                                    "sub2-Item 4",
                                    "sub2-Item five",
                                    "sub2-back <-"};






#ifdef DEF_EDIT_ITEMS                                    
int i16 = 1234;
float f = 1234.56;
bool  b = true;

uint8_t h = 12;
uint8_t m = 23;
uint8_t s = 55;

uint8_t day = 1;
uint8_t month = 2;
uint16_t year = 2019;

uint8_t ip_nmb[4] = {192,168,1,2};

#define BR_CNT      10

const PROGMEM uint32_t  ser_br[BR_CNT]={1200,2400,4800,9600,19200,38400,57600,74880,115200,230400};

const PROGMEM int16_t  i_16[BR_CNT]={-1200,2400,-4800,9600,19200,-384,576,-748,1152,-2304};


/*
#if defined(ARDUINO_ARCH_AVR)
edit_ip edt_ip(F("gateway"),ip_nmb[0],ip_nmb[1],ip_nmb[2],ip_nmb[3]);
edit_bool edt_bool(F("bool"),b);
edit_numb<int> edt_int(NMB_S_INT,F("int16"),i16,-4321,9999);
edit_numb<float> edt_float(NMB_FLOAT,F("float"),f,1200.0,1300.0);
edit_list edt_list(F("list"),mm_items[0],3,mm_items_len);
edit_time edt_time(F("time"),h,m,s);
edit_date edt_date(F("date"),year,month,day);
#else

edit_ip edt_ip("gateway",ip_nmb[0],ip_nmb[1],ip_nmb[2],ip_nmb[3]);
edit_bool edt_bool("bool",b);
edit_numb<int> edt_int(NMB_S_INT,"int16",i16,-4321,9999);
edit_numb<float> edt_float(NMB_FLOAT,"float",f,1200.0,1300.0);
edit_list edt_list("list",mm_items[0],3,mm_items_len);
edit_time edt_time("time",h,m,s);
edit_date edt_date("date",year,month,day);

#endif
*/
#endif //ifdef DEF_EDIT_ITEMS

                                    
#endif //ifndef __TEST_DATA_H__

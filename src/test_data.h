#include <edit_menu.h>
#ifndef __TEST_DATA_H__

#define __TEST_DATA_H__


#define DEF_EDIT_ITEMS  1




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

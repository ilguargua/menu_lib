#ifndef __TEST_DATA_H__

#define __TEST_DATA_H__

//#ifdef ARDUINO
//#include "../src/basic_menu.h"
//#else
//#include "basic_menu.h"
//#endif

#define MNU_PB1     A0
#define MNU_PB2     A1
#define MNU_PB3     A2
#define MNU_PB4     A3

#define MNU_PB_UP   MNU_PB1
#define MNU_PB_DN   MNU_PB2
#define MNU_PB_CH   MNU_PB3

typedef enum{
    M_PB_NONE,
    M_PB_UP,
    M_PB_DN,
    M_PB_CH
} m_pb_stat_t;



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


                                    
#endif

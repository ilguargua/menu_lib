#ifndef __BASIC_INPUTS_H__

#define __BASIC_INPUTS_H__

#include <edit_menu.h>

//Input methods
//uncomment only one of the following
#define MNU_USE_ENCODER     1
//#define MNU_USE_PB          1
//#define MNU_USE_SERIAL      1


//If using MNU_USE_PB how many push buttons to use
//uncomment only one
#define MNU_3_BTNS          3   //up down ok
//#define MNU_4_BTNS          4   //up down ok back (right)
//#define MNU_5_BTNS          5   //up down ok left right


//inputs pins defs
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


//returned values
typedef enum{
    M_PB_NONE,
    M_PB_UP,
    M_PB_DN,
    M_PB_OK,
    M_PB_RI,
    M_PB_LE
} m_input_vals_t;

#define MNU_INP_LEN     5

#define PB_ACTIVE       LOW
#define PB_NOT_ACTIVE   HIGH
#define SER_INP_DEV     Serial

#ifdef MNU_USE_PB
uint8_t           pb_prev_st[MNU_INP_LEN] = {PB_NOT_ACTIVE,PB_NOT_ACTIVE,PB_NOT_ACTIVE,PB_NOT_ACTIVE,PB_NOT_ACTIVE};
const uint8_t     btns[MNU_INP_LEN] = {MNU_PB_UP,MNU_PB_DN,MNU_PB_OK,MNU_PB_RI,MNU_PB_LE};

void pb_setup(){
    uint8_t mode = PB_ACTIVE == LOW ? INPUT_PULLUP : INPUT;
    pinMode(MNU_PB1,mode);
    pinMode(MNU_PB2,mode);
    pinMode(MNU_PB3,mode);
    pinMode(MNU_PB4,mode);
    pinMode(MNU_PB5,mode);
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

uint8_t ck_btns(){
    uint8_t res = M_PB_NONE;
    for(uint8_t i =0;i<MNU_INP_LEN;i++){
        res = pb_check(i);
        if(res != M_PB_NONE) return res;
    }
    return res;
}
#endif

#ifdef MNU_USE_SERIAL
const char ser_cod[MNU_INP_LEN] = {'u','d','o','l','r'};

uint8_t serial_check(){//Stream &serial){
    char c = 0;
    if(SER_INP_DEV.available()){
        c = SER_INP_DEV.read();
        for(uint8_t i=0;i<MNU_INP_LEN;i++){
            if(ser_cod[i] == c){
                while(SER_INP_DEV.available()) SER_INP_DEV.read();
                return i+1;
            }
        }
    }
    //while(Serial.available()) Serial.read();
    return M_PB_NONE;
}
#endif

#ifdef MNU_USE_ENCODER
struct enc_data{
    volatile uint8_t pa=0;
    volatile uint8_t pb=0;
    volatile int8_t  value=0;
    uint8_t          ppb=PB_NOT_ACTIVE;
} enc;

void enc_a_isr(){
    if(digitalRead(ENC_CH_B) == PB_ACTIVE and enc.pb) {
        enc.value -= 1;
        enc.pb = 0;
        enc.pa = 0;
        return;
    }
    if(digitalRead(ENC_CH_A) == PB_ACTIVE and enc.pa == 0){ 
        enc.pa = 1;
        enc.pb = 0;
    }  //reset pb??
}

void enc_b_isr(){
    if(digitalRead(ENC_CH_A) == PB_ACTIVE and enc.pa) {
        enc.value += 1;
        enc.pa = 0;
        enc.pb = 0;
        return;
    }
    if(digitalRead(ENC_CH_B) == PB_ACTIVE and enc.pb == 0){ 
        enc.pb = 1;
        enc.pa = 0;
    }
}

void enc_timed_isr(){
    uint8_t a = digitalRead(ENC_CH_A) == PB_ACTIVE ? 1 : 0;
    uint8_t b = digitalRead(ENC_CH_B) == PB_ACTIVE ? 1 : 0;
    uint8_t r = 0;
    r = (enc.pb*8)+(enc.pa*4)+(b*2)+a;
    switch(r){
        case 1:             // A    B   PA  PB
                            // 1    0   0   0   = 1
            
            enc.pa = 1;     // 1    0   1   0   = 5
            break;
        case 2:             // A    B   PA  PB
                            // 0    1   0   0   = 2

            enc.pb = 1;     // 0    1   0   1   = 10            
            break;
        case 7:             // A    B   PA  PB
                            // 1    1   1   0
            enc.pb = 0;
            enc.pa = 0;
            enc.value += 1;
            break;
        case 11:        // A    B   PA  PB
                        // 1    1   0   1
            enc.value -= 1;
            enc.pa = 0;
            enc.pb = 0;
            break;
        
        case 4:
            enc.pa = 0;
            break;
        case 8:
            enc.pb = 0;
            break;
    }
}

uint8_t encoder_check(){
    uint8_t ret = M_PB_NONE;
    if(digitalRead(ENC_PB) == PB_ACTIVE){
        if(enc.ppb == PB_NOT_ACTIVE) enc.ppb = PB_ACTIVE;
        ret = M_PB_NONE;
    }
    else{
        if(enc.ppb == PB_ACTIVE){
            enc.ppb = PB_NOT_ACTIVE;
            ret = M_PB_OK;
        }
    }
    if(enc.value !=0){
        if(enc.value > 0) ret = M_PB_UP;
        else ret = M_PB_DN;
        enc.value = 0;
    }
    return ret;
}

void enc_setup(){
    uint8_t mode = PB_ACTIVE == LOW ? INPUT_PULLUP : INPUT;
    pinMode(ENC_CH_A,mode);
    pinMode(ENC_CH_B,mode);
    attachInterrupt(digitalPinToInterrupt(ENC_CH_A), enc_a_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(ENC_CH_B), enc_b_isr, FALLING);
}
#endif

uint8_t input_check(){
#ifdef MNU_USE_PB
    return ck_btns();
#endif
#ifdef MNU_USE_ENCODER
    return encoder_check();
#endif
#ifdef MNU_USE_SERIAL
    return serial_check();
#endif
    return M_PB_NONE;
}

int8_t menu_loop(basic_menu *mnu){
    int8_t ret = -1;
    switch(input_check()){
        case M_PB_UP:
        case M_PB_LE:
            mnu->move_prev();
            //mnu->draw_menu();
            break;
        case M_PB_DN:
        case M_PB_RI:
            mnu->move_next();
            //mnu->draw_menu();
            break;
        case M_PB_OK:
            ret = mnu->cur_item;
            break;
    }
    return ret;
}

uint8_t edit_item_loop(edit_item *cur_item){
    uint8_t ret = 0; //0 - editing, no change 1 - editing, need disp update  2 - end editing
    switch(input_check()){
        case M_PB_UP:
            cur_item->set_next_value();
            ret = 1;
            break;
        case M_PB_DN:
            cur_item->set_prev_value();
            ret = 1;
            break;
        case M_PB_OK:
#if defined(MNU_3_BTNS) || defined(MNU_4_BTNS)  // OK btn in 3/4 btns mode shift digit (if EDT_MODE_DIGIT) to left or exit(if no more digit)
            if(cur_item->edt_mode == EDT_MODE_STEP){
                ret = 2;
            }
            else{
                if(cur_item->set_next_digit()){ 
                    ret = 1;
                }
                else{ 
                    cur_item->reset_step();
                    ret = 2;
                }
            }
#else   //with 5 btns just exit
            ret = 2;
#endif            
            break;
        case M_PB_RI:  // right btn shift digit to right or exit if no more digit
            if(cur_item->edt_mode == EDT_MODE_DIGIT){
                if(cur_item->set_prev_digit()){ 
                    ret = 1;
                }
                else{
                    cur_item->reset_step();
                    ret = 2;
                }
            }
            break;
        case M_PB_LE:  // left btn shift digit to left or exit if no more digit
            if(cur_item->edt_mode == EDT_MODE_DIGIT){
                if(cur_item->set_next_digit()){ 
                    ret = 1;
                }
                else{
                    cur_item->reset_step();
                    ret = 2;
                }
            }
            break;

    }
    
}

int8_t edit_menu_loop(edit_menu *mm){
    int8_t ret = -1;     //-1 - editing, no change -2 - editing, need disp update  0 or pos. nmb - exit with item nmb
    switch(input_check()){
        case M_PB_UP:
            if(mm->state == EDT_STATE_MENU){ 
                mm->move_prev();
                //mm->draw_menu();
            }
            else{ 
                mm->edit_set_next();
                //mm->edit_current();
            }
            break;
        case M_PB_DN:
            if(mm->state == EDT_STATE_MENU){
                mm->move_next();
                //mm->draw_menu();
            }
            else{ 
                mm->edit_set_prev();
                //mm->edit_current();
            }
            break;
        case M_PB_OK:
            //Serial.println("M_PB_CH pressed");
            if(mm->state == EDT_STATE_MENU){
                if(mm->get_cur_item()->edt_mode == EDT_MODE_NONE) ret = mm->cur_item;
                else mm->edit_current();
            }
            else{
#if defined(MNU_3_BTNS) || defined(MNU_4_BTNS)  // OK btn in 3/4 btns mode shift digit (if EDT_MODE_DIGIT) to left or exit(if no more digit)                
                if(mm->get_cur_item()->edt_mode == EDT_MODE_DIGIT and mm->set_next_digit()){ 
                    mm->edit_current();
                }
                else{ 
                    ret = mm->cur_item;
                    mm->state = EDT_STATE_MENU;
                    mm->draw_menu();
                }
#else
                //ret = mm->cur_item;
                mm->state = EDT_STATE_MENU;
                mm->draw_menu();
#endif
            }
            break;
        case M_PB_RI:
            //Serial.println(F("M_PB_BK pressed"));
            if(mm->state == EDT_STATE_EDIT){
                if(mm->get_cur_item()->edt_mode == EDT_MODE_DIGIT and  mm->set_prev_digit()){ 
                    mm->edit_current();
                }
                else{
                    //mm->state = EDT_STATE_MENU;
                    mm->draw_menu();
                }
            }
            break;
        case M_PB_LE:
            //Serial.println(F("M_PB_BK pressed"));
            if(mm->state == EDT_STATE_EDIT){
                if(mm->get_cur_item()->edt_mode == EDT_MODE_DIGIT and  mm->set_next_digit()){ 
                    mm->edit_current();
                }
                else{
                    //mm->state = EDT_STATE_MENU;
                    mm->draw_menu();
                }
            }
            break;
        
    }
    
}

#endif // ifndef __BASIC_INPUTS_H__

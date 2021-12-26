/*
 * basic_menu test using a Nextion interface
 * 02/12/2021
 */




#include <SoftwareSerial.h>
#include <basic_menu.h>
#include <edit_menu.h>
#include <nextion_display.h>
#include <test_data.h>



SoftwareSerial intf(10,11);  //we use software serial for display, so Serial can be used for debug
                             //In this case: blu wire to pin 10, yellow wire to pin 11 


basic_menu bm;
nextion_display disp;

const PROGMEM char pm_str[][4] = {"uno","due","tre","sei"};


typedef struct{
    PGM_P      base;
    uint8_t         cnt;
    uint8_t         len;
    PGM_P operator[](int8_t i){
        i=constrain(i,0,cnt-1);
        if(base != nullptr) return base + (len * i);
        else return nullptr;
    };
} pgm_arr_t;


pgm_arr_t pg;

void setup(){
    char buf[60];
    memset(buf,0,40);
    intf.begin(38400);  //This have to match the nextion baudrate
    
    pg.base = pm_str[0];
    pg.cnt = 4;
    pg.len = 4;
    
    
    
    //nextion_display disp(&intf,5,16,0,18,10);
    //basic_menu bm;
    Serial.begin(115200);
    Serial.println("Program begin..");
    delay(500);
    
    //sprintf(buf,"%S",pg[2]);
    strcpy_P(buf,pg[1]);
    Serial.println(buf);
    
    memset(buf,0,40);
    //set input buttons
    pinMode(MNU_PB_UP,INPUT);
    pinMode(MNU_PB_DN,INPUT);
    pinMode(MNU_PB_CH,INPUT);
    
    disp.init(&intf,5,16,0,14,8);
    Serial.print("nxt_buf : ");
    for(uint8_t i=0;i<10;i++){
        Serial.print((uint8_t)disp.nxt_buf[i],HEX);
        Serial.print(" ");
    }
    Serial.println();
    
    snprintf(buf,40,"rest%c%c%c",NXT_MSG_END);
    intf.print(buf);
    delay(1000);
    memset(buf,0,40);
    snprintf(buf,40,"b[3].val=50%c%c%c",NXT_MSG_END);
    intf.print(buf);
    
    //while(1){};
    Serial.print("d_rows : ");
    Serial.println(disp.d_rows);
    Serial.print("d_cols : ");
    Serial.println(disp.d_cols);
    
    //memset(buf,0,60);
    //snprintf(buf,60,"xstr 121,73,50,18,0,0,65535,1,1,1,\"hello\"%c%c%c",NXT_MSG_END);
    //intf.print(buf);
    
    disp.clear_display();
    //Serial.print("conv_buf : ");
    //Serial.println(disp.conv_buf);
    
    //disp.print(0,0,"hello");
    
    
    
    
    bm.set_device(&disp);
    bm.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    bm.set_options(M_PRINT_CLEAR);
    bm.set_rows(5);
    
    bm.draw_menu();
    //delay(2000);
    //disp.clear_row(4,0);
    Serial.print("conv_buf : ");
    Serial.println(disp.conv_buf);
    Serial.print("menu_x : ");
    Serial.println(disp.menu_x);
    Serial.print("menu_y : ");
    Serial.println(disp.menu_y);
    Serial.print("row_h : ");
    Serial.println(disp.row_h);
    Serial.print("col_l : ");
    Serial.println(disp.col_l);
    Serial.print("disp_rows : ");
    Serial.println(bm.disp_rows);
    
    
    
    

}



void loop(){
    switch(chk_pb()){
        case M_PB_UP:
            bm.move_prev();
            break;
        case M_PB_DN:
            bm.move_next();
            break;
        case M_PB_CH:
            disp.restore_display();
            break;
    }
}


int chk_pb(){
    char b = 0;
    if(Serial.available()){
        b = Serial.read();
        switch(b){
            case 'u':
                return M_PB_UP;
                break;
            case 'd':
                return M_PB_DN;
                break;
            case 'c':
                return M_PB_CH;
                break;
        }
        while(Serial.available()) Serial.read();
    }
    return M_PB_NONE;
    if(digitalRead(MNU_PB_UP) == LOW){
        while(digitalRead(MNU_PB_UP) == LOW){}
        return M_PB_UP;
    }
    if(digitalRead(MNU_PB_DN) == LOW){
        while(digitalRead(MNU_PB_DN) == LOW){}
        return M_PB_DN;
    }
    if(digitalRead(MNU_PB_CH) == LOW) {
        while(digitalRead(MNU_PB_CH) == LOW){}
        return M_PB_CH;
    }
    return M_PB_NONE;
}

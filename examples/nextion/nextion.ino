/*
 * basic_menu test using a Nextion interface
 * 02/12/2021
 */




#include <SoftwareSerial.h>
#include <basic_menu.h>
#include <edit_menu.h>
#include <nextion_display.h>
#include <test_data.h>
//#include <u8x8_display.h>


SoftwareSerial intf(10,11);  //we use software serial for display, so Serial can be used for debug
                             //In this case: blu wire to pin 10, yellow wire to pin 11 


basic_menu bm;
nextion_display disp;

//const PROGMEM uint32_t  ser_br[]={1200,2400,4800,9600,19200,38400,57600,74880,115200,230400};
//uint32_t  ser_br[]={1200,2400,4800,9600,19200,38400,57600,74880,115200,230400};
//uint32_t  tt = 1234;

//U8X8_SSD1306_128X64_NONAME_HW_I2C  u8x8_dev;

//u8x8_display  disp;

edit_menu mm(&disp);

edit_ip edt_ip(PSTR("gateway"),ip_nmb[0],ip_nmb[1],ip_nmb[2],ip_nmb[3]);



void setup(){
    char buf[60];
    memset(buf,0,40);
    intf.begin(38400);  //This have to match the nextion baudrate
    
    //basic_menu bm;
    Serial.begin(115200);
    Serial.println("Program begin..");
    //pinMode(13,INPUT_PULLUP);
    //pinMode(13,OUTPUT);
    delay(500);
    //digitalWrite(13,HIGH);
    
    //set input buttons
    pinMode(MNU_PB_UP,INPUT);
    pinMode(MNU_PB_DN,INPUT);
    pinMode(MNU_PB_CH,INPUT);


    disp.init(&intf,5,16,0,14,8);
    snprintf_P(buf,40,PSTR("rest%c%c%c"),NXT_MSG_END);
    intf.print(buf);
    delay(1000);
    
    
    //edt_float.set_edit_mode(EDT_MODE_DIGIT);
    //edt_int.set_edit_mode(EDT_MODE_DIGIT);
    //edt_date.set_fmt(DT_DMY);
    
    //mm.add_item(&edt_time);
    //mm.add_item(&edt_date);
    //mm.add_item(&edt_bool);
    //mm.add_item(&edt_int);
    //mm.add_item(&edt_float);
    //mm.add_item(&edt_list);
    
    
    
    
    /*
    memset(buf,0,40);
    snprintf(buf,40,"b[3].val=50%c%c%c",NXT_MSG_END);
    intf.print(buf);
    */
    //while(1){};
    
    /*
    Serial.print("d_rows : ");
    Serial.println(disp.d_rows);
    Serial.print("d_cols : ");
    Serial.println(disp.d_cols);
    */
    //memset(buf,0,60);
    //snprintf(buf,60,"xstr 121,73,50,18,0,0,65535,1,1,1,\"hello\"%c%c%c",NXT_MSG_END);
    //intf.print(buf);
    
    disp.clear_display();
    //Serial.print("conv_buf : ");
    //Serial.println(disp.conv_buf);
    
    //disp.print(0,0,"hello");
    
    mm.set_options(M_PRINT_CLEAR);
    //mm.set_rows(1);
    mm.set_rows(6);
    //mm.set_title("Main menu");
    mm.draw_menu();
    
    /*
    for(uint8_t i=0;i<mm.items.cnt;i++){
        Serial.print("mm.edt_items[");
        Serial.print(i);
        Serial.print("].conv_buf = >");
        Serial.print(mm.edt_items[i]->conv_buf);
        Serial.println("<");
    }
    */
    //Serial.println(disp.cols);
    /*
    bm.set_device(&disp);
    bm.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    bm.set_options(M_PRINT_CLEAR);
    bm.set_rows(5);
    bm.set_title(F("Main menu"));
    
    bm.draw_menu();
    */
    //delay(2000);
    //disp.clear_row(4,0);
    /*
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
    */
    
    
    

}



void loop(){
    
    switch(chk_pb()){
        case M_PB_UP:
            if(mm.state == EDT_STATE_MENU){ 
                mm.move_prev();
            }
            else{ 
                mm.edit_set_next();
            }
            break;
        case M_PB_DN:
            if(mm.state == EDT_STATE_MENU){
                mm.move_next();
            }
            else{ 
                mm.edit_set_prev();
            }

            break;
        case M_PB_CH:
            if(mm.state == EDT_STATE_MENU){
                mm.edit_current();
            }
            else{
                if(mm.set_next_digit()){ 
                    mm.edit_current();
                }
                else{ 
                    mm.draw_menu();
                }
            }
            break;
        
    }
    
    /*
    static uint8_t cur_menu = 0; //0 main , 1 sub
    
    switch(chk_pb()){
        case M_PB_UP:
            bm.move_prev();
            break;
        case M_PB_DN:
            bm.move_next();
            break;
        case M_PB_CH:
            //disp.restore_display();
            if(cur_menu==0){
                switch(bm.cur_item){
                    case 0:
                        bm.set_items(s1_items[0],s1_cnt,s1_len);
                        bm.set_title(F("SubMenu 1"));
                        bm.draw_menu();
                        cur_menu = 1;
                        break;
                    case 1:
                        bm.set_items(s2_items[0],s2_cnt,s2_len);
                        bm.set_title(F("SubMenu 2"));
                        bm.draw_menu();
                        cur_menu = 1;
                        break;
                        
                }
            }
            else{
                if(bm.cur_item==bm.items.cnt-1){
                    bm.set_items(mm_items[0],mm_items_cnt,mm_items_len);
                    bm.set_title(F("Main menu"));
                    bm.draw_menu();
                    cur_menu = 0;
                }
            }
            break;
    }
    */
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

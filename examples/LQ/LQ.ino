/*
 * basic_menu test using a LiquidCrystal interface
 * 02/12/2021
 */



#include <basic_menu.h>
#include <lq_display.h>
#include <edit_menu.h>

#include <test_data.h>


LiquidCrystal lcd(8,9,4,5,6,7);

//basic_menu mm;
lq_display dev(&lcd,20,4);
edit_menu mm(&dev);

int edt_var = 1234;
float f = 1234.56;
bool  b = true;

uint8_t h = 12;
uint8_t m = 23;
uint8_t s = 55;

uint8_t day = 1;
uint8_t month = 2;
uint16_t year = 2019;

/*
edit_item *edt_bool = new edit_bool("bool",b);
edit_item *edt_int = new edit_numb<int>(NMB_S_INT,"int16",edt_var,-4321,9999);
edit_item *edt_float = new edit_numb<float>(NMB_FLOAT,"float",f,1200,1300);
edit_item *edt_list = new edit_list("list",mm_items[0],3,mm_items_len);
*/

edit_bool edt_bool("bool",b);
//edit_numb<int> edt_int(NMB_S_INT,"int16",edt_var,-4321,9999);
//edit_numb<float> edt_float(NMB_FLOAT,"float",f,1200,1300);
edit_list edt_list("list i",mm_items[0],3,mm_items_len);
edit_time edt_time("time edt",h,m,s);
edit_date edt_date("date edit",year,month,day);
//pb_collection btns;

//push_button up = push_button(MNU_PB_UP,LOW,M_PB_UP);

void setup(){
    
    //Serial.begin(115200);
    //Serial.println("Program begin");
    char buf[20];
    memset(buf,0,20);
    pinMode(MNU_PB_UP,INPUT);
    pinMode(MNU_PB_DN,INPUT);
    pinMode(MNU_PB_CH,INPUT);
    //btns.add_btn(MNU_PB_UP,LOW,M_PB_UP);
    //btns.add_btn(&up);
    //btns.add_btn(MNU_PB_DN,LOW,M_PB_DN);
    //btns.add_btn(MNU_PB_CH,LOW,M_PB_CH);
    
    lcd.begin(20,4);
    lcd.print("HELLO world!");
    //while(1){};
    
    //edt_float.set_edit_mode(EDT_MODE_DIGIT);
    //edt_int.set_edit_mode(EDT_MODE_DIGIT);
    edt_date.set_fmt(DT_DMY);
    mm.add_item(&edt_time);
    mm.add_item(&edt_bool);
    //mm.add_item(&edt_int);
   // mm.add_item(&edt_date);
    //mm.add_item(&edt_list);
    //mm.set_device(&dev);
    //mm.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    mm.set_options(M_PRINT_CLEAR);
    //mm.set_rows(1);
    //mm.set_rows(4);
    //mm.set_title("Main menu");
    mm.draw_menu();
    //lcd.setCursor(0,2);
    /*
    Serial.print("up.ps ");
    Serial.println(up.ps);
    Serial.print("up.value ");
    Serial.println(up.value);

    Serial.print("up.get_status() ");
    Serial.println(up.get_status());
    */
    //int a=2,b=3;
    //swap(a,b);

}


void loop(){
    
    switch(chk_pb()){
    //switch(btns.get_btns_status()){
        case M_PB_UP:
            //Serial.println("M_PB_UP pressed");
            if(mm.state == EDT_STATE_MENU){ 
                mm.move_prev();
                /*
                Serial.print("mm.state = ");
                Serial.println(mm.state);
                Serial.println("mm.move_prev()");
                */
            }
            else{ 
                mm.edit_set_next();
                /*
                Serial.print("mm.state = ");
                Serial.println(mm.state);
                Serial.println("mm.edit_set_next()");
                */
            }
            //Serial.println();
            break;
        case M_PB_DN:
            //Serial.println("M_PB_DN pressed");
            if(mm.state == EDT_STATE_MENU){
                mm.move_next();
                /*
                Serial.print("mm.state = ");
                Serial.println(mm.state);
                Serial.println("mm.move_next()");
                */
            }
            else{ 
                mm.edit_set_prev();
                /*
                Serial.print("mm.state = ");
                Serial.println(mm.state);
                Serial.println("mm.edit_set_prev()");
                */
            }
            //Serial.println();
            break;
        case M_PB_CH:
            //Serial.println("M_PB_CH pressed");
            if(mm.state == EDT_STATE_MENU){
                mm.edit_current();
                /*
                Serial.print("mm.state = ");
                Serial.println(mm.state);
                Serial.println("mm.edit_current()");
                */
            }
            else{
                if(mm.set_next_digit()){ 
                    mm.edit_current();
                    //Serial.println("mm.set_next_digit() true");
                    //Serial.println("mm.edit_current()");
                }
                else{ 
                    mm.draw_menu();
                    //Serial.println("mm.set_next_digit() false");
                    //Serial.println("mm.draw_menu()");
                }
            }
            //Serial.println();
            break;
        
    }
    
}

int chk_pb(){
    static uint8_t st[4] = {HIGH,HIGH,HIGH,HIGH};
    
    if(digitalRead(MNU_PB_UP) == LOW){
        if(st[0] == HIGH) st[0] = LOW;
        return M_PB_NONE;
    }
    else{
        if(st[0] == LOW){
            st[0] = HIGH;
            return M_PB_UP;
        }
    }
    
    if(digitalRead(MNU_PB_DN) == LOW){
        if(st[1] == HIGH) st[1] = LOW;
        return M_PB_NONE;
    }
    else{
        if(st[1] == LOW){
            st[1] = HIGH;
            return M_PB_DN;
        }
    }
    
    if(digitalRead(MNU_PB_CH) == LOW) {
        if(st[2] == HIGH) st[2] = LOW;
        return M_PB_NONE;
    }
    else{
        if(st[2] == LOW){
            st[2] = HIGH;
            return M_PB_CH;
        }
    }
    
    return M_PB_NONE;
}


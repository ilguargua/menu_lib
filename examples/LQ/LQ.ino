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

/*
int edt_var = 1234;
float f = 1234.56;
bool  b = true;

uint8_t h = 12;
uint8_t m = 23;
uint8_t s = 55;

uint8_t day = 1;
uint8_t month = 2;
uint16_t year = 2019;
*/
/*
edit_item *edt_bool = new edit_bool("bool",b);
edit_item *edt_int = new edit_numb<int>(NMB_S_INT,"int16",edt_var,-4321,9999);
edit_item *edt_float = new edit_numb<float>(NMB_FLOAT,"float",f,1200,1300);
edit_item *edt_list = new edit_list("list",mm_items[0],3,mm_items_len);
*/

/*
edit_bool edt_bool("bool",b);
edit_numb<int> edt_int(NMB_S_INT,"int16",edt_var,-4321,9999);
edit_numb<float> edt_float(NMB_FLOAT,"float",f,1200.0,1300.0);
edit_list edt_list("list",mm_items[0],3,mm_items_len);
edit_time edt_time("time",h,m,s);
edit_date edt_date("date",year,month,day);
*/
//pb_collection btns;

//push_button up = push_button(MNU_PB_UP,LOW,M_PB_UP);

edit_numb<int> edt_int(NMB_S_INT,i16);//,-4321,9999);
edit_numb<float> edt_float(NMB_FLOAT,f);//,1200.0,1300.0);
edit_date edt_date(year,month,day);
edit_ip edt_ip(ip_nmb[0],ip_nmb[1],ip_nmb[2],ip_nmb[3]);

edit_item   *cur_item = nullptr;

void setup(){
    
    Serial.begin(115200);
    Serial.println("Program begin");
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
    
    /*
    Serial.print(F("float.nmb_len : "));
    Serial.println(edt_float.nmb_len);
    Serial.print(F("float.cur_digit : "));
    Serial.println(edt_float.cur_digit);
    
    edt_float.set_edit_mode(EDT_MODE_DIGIT);
    edt_int.set_edit_mode(EDT_MODE_DIGIT);
    edt_date.set_fmt(DT_DMY);
    mm.add_item(&edt_time);
    mm.add_item(&edt_bool);
    mm.add_item(&edt_int);
    mm.add_item(&edt_float);
    Serial.print(F("float.nmb_len : "));
    Serial.println(edt_float.nmb_len);
    Serial.print(F("float.cur_digit : "));
    Serial.println(edt_float.cur_digit);
    */
    
    edt_float.set_edit_mode(EDT_MODE_DIGIT);
    //edt_int.set_edit_mode(EDT_MODE_DIGIT);
    edt_int.set_min(1000);
    edt_int.set_step(10);
    edt_date.set_fmt(DT_DMY);
    //Serial.print(F("int options : "));
    //Serial.println(edt_int.options);
    Serial.print(F("float.cur_digit : "));
    Serial.println(edt_float.cur_digit);
    //dtostrf(edt_float.value,-19,2,buf);
    //Serial.println(buf);
    Serial.println(edt_float.get_txt_value());
    
    mm.add_item(&edt_int,F("int16"));
    mm.add_item(&edt_date,F("date"));
    mm.add_item(&edt_ip,F("ip"));
    mm.add_item(&edt_float,F("float"));
    
    cur_item = &edt_date;
    edit_cur_item();
    Serial.print(F("*float.cur_digit : "));
    Serial.println(edt_float.cur_digit);
    
    //while(1){};
    // mm.add_item(&edt_date);
    //mm.add_item(&edt_list);
    //mm.set_device(&dev);
    //mm.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    mm.set_options(M_PRINT_CLEAR);
    //mm.set_rows(1);
    //mm.set_rows(4);
    //mm.set_title("Main menu");
    //mm.draw_menu();
    
    /*
    Serial.print(F("float.nmb_len : "));
    Serial.println(edt_float.nmb_len);
    Serial.print(F("float.cur_digit : "));
    Serial.println(edt_float.cur_digit);
    */
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
    edit_item_loop();
    
}


void edit_item_loop(){
    switch(chk_pb()){
        case M_PB_UP:
            cur_item->set_next_value();
            edit_cur_item();
            break;
        case M_PB_DN:
            cur_item->set_prev_value();
            edit_cur_item();
            break;
        case M_PB_CH:
            if(cur_item->edt_mode == EDT_MODE_STEP){
                dev.clear_display();
                //cur_item->reset_step();
            }
            else{
                if(cur_item->set_next_digit()){ 
                    edit_cur_item();
                }
                else{ 
                    dev.clear_display();
                    cur_item->reset_step();
                }
            }
            break;
        case M_PB_BK:
            //Serial.println(F("M_PB_BK pressed"));
            if(cur_item->edt_mode == EDT_MODE_DIGIT){
                if(cur_item->set_prev_digit()){ 
                    edit_cur_item();
                }
                else{
                    dev.clear_display();
                    cur_item->reset_step();
                }
            }
            break;
        
    }
    
}


void edit_cur_item(){
    if(cur_item != nullptr) cur_item->edit(&dev,2,0,0,1);
}

void edit_menu_loop(){
    switch(chk_pb()){
        case M_PB_UP:
            //Serial.println("M_PB_UP pressed");
            if(mm.state == EDT_STATE_MENU){ 
                mm.move_prev();
            }
            else{ 
                mm.edit_set_next();
            }
            break;
        case M_PB_DN:
            //Serial.println("M_PB_DN pressed");
            if(mm.state == EDT_STATE_MENU){
                mm.move_next();
            }
            else{ 
                mm.edit_set_prev();
            }
            break;
        case M_PB_CH:
            //Serial.println("M_PB_CH pressed");
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
        case M_PB_BK:
            //Serial.println(F("M_PB_BK pressed"));
            if(mm.state == EDT_STATE_EDIT){
                if(mm.set_prev_digit()){ 
                    mm.edit_current();
                }
                else{
                    mm.draw_menu();
                }
            }
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

    if(digitalRead(MNU_PB_BK) == LOW) {
        if(st[3] == HIGH) st[3] = LOW;
        return M_PB_NONE;
    }
    else{
        if(st[3] == LOW){
            st[3] = HIGH;
            return M_PB_BK;
        }
    }
    
    return M_PB_NONE;
}


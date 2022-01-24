/*
 * basic_menu test using a LiquidCrystal interface
 * 02/12/2021
 */


#include <Wire.h>
//#include <basic_menu.h>
#include <lq_display.h>
//#include <test_data.h>
//#include <basic_inputs.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
//#include <edit_menu.h>


//LiquidCrystal lcd(8,9,4,5,6,7);

hd44780_I2Cexp lcd;


lq_display disp(&lcd,20,4);


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

//edit_numb<int> edt_int(NMB_S_INT,i16);//,-4321,9999);
//edit_numb<float> edt_float(NMB_FLOAT,f);//,1200.0,1300.0);
//edit_date edt_date(year,month,day);
//edit_ip edt_ip(ip_nmb[0],ip_nmb[1],ip_nmb[2],ip_nmb[3]);

//edit_item   *cur_item = nullptr;


void setup(){
    
    Serial.begin(115200);
    Serial.println("Program begin");
    
    
    
    //quick test of device
    lcd.begin(20,4);
    lcd.print("HELLO world!");
    delay(2000);
    
    disp.clear_display();
    disp.print(0,0,"Normal text line");
    disp.print(1,0,"Highlite txt line",1);
    disp.print(2,0,"Hilite digit 123456");
    disp.print(2,15,"3",2);

}


void loop(){
    
}



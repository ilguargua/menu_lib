

#include <basic_menu.h>
#include <test_data.h>
//#include <serial_display.h>
//#include <edit_menu.h>
#include <u8x8_display.h>
#include <basic_inputs.h>

//#include <test_data.h>




U8X8_SSD1306_128X64_NONAME_HW_I2C  u8x8_dev;


u8x8_display  disp;




basic_menu my_menu;


void setup(){
    Serial.begin(115200);
    Serial.println(F("program begin..."));

    u8x8_dev.setI2CAddress(0x78);
    u8x8_dev.begin();
    u8x8_dev.setFont(u8x8_font_chroma48medium8_r);
    disp.set_device(&u8x8_dev);
    Serial.print("disp.rows = ");
    Serial.println(disp.rows);
    Serial.print("disp.cols = ");
    Serial.println(disp.cols);

#ifdef MNU_USE_ENCODER
    enc_setup();
#endif    

#ifdef MNU_USE_PB
    pb_setup();
#endif    
    
    my_menu.set_device(&disp);
    my_menu.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    my_menu.set_title(F("My menu"));
    my_menu.set_rows(4);
    my_menu.draw_menu();
    

}


void loop(){
    menu_loop(&my_menu);
    
}


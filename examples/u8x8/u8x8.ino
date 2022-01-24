
//#define M_BF_LEN 18
//#include <basic_menu.h>
//#include <serial_display.h>
#include <u8x8_display.h>

//#include <test_data.h>




U8X8_SSD1306_128X64_NONAME_HW_I2C  u8x8_dev;
//U8X8_SSD1306_128X64_NONAME_2ND_HW_I2C  u8x8_dev;
//U8X8_SSD1306_128X64_VCOMH0_HW_I2C  u8x8_dev;
//U8X8_SSD1306_128X64_VCOMH0_2ND_HW_I2C  u8x8_dev;


u8x8_display  disp;


void setup(){

    u8x8_dev.setI2CAddress(0x78);
    u8x8_dev.begin();
    u8x8_dev.setFont(u8x8_font_chroma48medium8_r);
    disp.set_device(&u8x8_dev);
    
    //quick test of device
    //u8x8_dev.clearBuffer();
    u8x8_dev.drawString(0,0,"HELLO world!");
    
    delay(2000);
    
    disp.clear_display();
    disp.print(0,0,"Normal text line");
    disp.print(1,0,"Hilite txt line",1);
    disp.print(2,0,"Hi. digit 123456");
    disp.print(2,12,"3",2);
    
    
    

}


void loop(){

    
}


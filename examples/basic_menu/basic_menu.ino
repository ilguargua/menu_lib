
//#define M_BF_LEN 18
#include <basic_menu.h>
#include <serial_display.h>
#include <u8x8_display.h>

#include <test_data.h>

//text_display  dumb;


U8X8_SSD1306_128X64_NONAME_HW_I2C  u8x8_dev;
//U8X8_SSD1306_128X64_NONAME_2ND_HW_I2C  u8x8_dev;
//U8X8_SSD1306_128X64_VCOMH0_HW_I2C  u8x8_dev;
//U8X8_SSD1306_128X64_VCOMH0_2ND_HW_I2C  u8x8_dev;

//u8x8_dev.setI2CAddress(0x78);

//u8x8_display dev(&u8x8_dev);

//serial_display dsp(&Serial);
//basic_menu    mm(&dsp,&mm_items[0][0],mm_items_cnt,mm_items_len,4);
basic_menu mm;

void setup(){
    //uint8_t edt_val = *(uint8_t *) edt_ptr->get_value_ptr();
    char buf[20];
    memset(buf,0,20);
    pinMode(MNU_PB_UP,INPUT);
    pinMode(MNU_PB_DN,INPUT);
    pinMode(MNU_PB_CH,INPUT);
    u8x8_dev.setI2CAddress(0x78);
    u8x8_dev.begin();
    u8x8_dev.setFont(u8x8_font_chroma48medium8_r);
    //u8x8_display dev(&u8x8_dev);
    //basic_menu    mm(&dev,&mm_items[0][0],mm_items_cnt,mm_items_len);
    //basic_menu    mm(&dev,mm_items[0],mm_items_cnt,mm_items_len);
    //basic_menu mm(M_U8X8,&u8x8_dev);
    mm.set_device(M_U8X8,&u8x8_dev);
    mm.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    mm.set_options(M_NO_ROLLOVER | M_PRINT_CLEAR);
    mm.set_rows(4);
    Serial.begin(115200);
    //Serial.println(edt_val);
    mm.draw_menu();
    Serial.println();
    sprintf(buf,"rows %i cols%i",mm.device->rows,mm.device->cols);
    u8x8_dev.drawString(0,6,buf);
    memset(buf,0,20);
    sprintf(buf,"d_rows %i",mm.disp_rows);
    u8x8_dev.drawString(0,7,buf);
    //mm.move_next();
    

}


void loop(){
    
    switch(chk_pb()){
        case M_PB_UP:
            mm.move_prev();
            break;
        case M_PB_DN:
            mm.move_next();
            break;
        
    }
    
    /*
    char buf[5];
    memset(buf,0,5);
    if(Serial.available()){
        Serial.readBytesUntil('\n',buf,4);
        switch(buf[0]){
            case 'n':
                //mm.move_next();
                Serial.println();
                break;
            case 'p':
                //mm.move_prev();
                Serial.println();
                break;
            case 'c':
                Serial.print("You choice is item ");
                //Serial.println(mm.cur_item);
                Serial.println();
                break;
        }
    }
    */
}

int chk_pb(){
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

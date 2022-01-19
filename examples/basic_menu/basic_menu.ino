

#include <basic_menu.h>

//#include <test_data.h>
//#include <serial_display.h>
#include <edit_menu.h>





//text_display  dumb;

//serial_display dsp(&Serial);

//basic_menu mm;

edit_numb_list<uint32_t>  edt("n",ser_br,10);
edit_numb_list<int16_t>  edt_16("t",i_16,10);

void setup(){
    Serial.begin(115200);
    for(uint8_t i=0;i<BR_CNT;i++){
        Serial.println((int16_t) pgm_read_word(i_16+i));
        Serial.println(pgm_read_word(edt_16.base+i));
    }
    
    //edt.set_next_value();
    /*
    Serial.println();
    Serial.println(edt_16.size);
    Serial.println(edt_16.cur_item);
    edt_16.set_next_value();
    Serial.println(edt_16.cur_item);
    Serial.println(edt_16.get_txt_value());
    */
    
    /*
    mm.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    mm.set_device(&dsp);
    mm.set_title(F("main menu"));
    mm.draw_menu();
    mm.move_next();
    //const char *t = mm.get_row(0);
    mm.draw_menu();
    mm.set_items(s1_items[0],s1_cnt,s1_len);
    mm.set_title(F("sub menu 1"));
    mm.draw_menu();
    mm.move_prev();
    mm.draw_menu();
    mm.set_items(s2_items[0],s2_cnt,s2_len);
    mm.draw_menu();
    mm.move_next();
    mm.draw_menu();
    */

}


void loop(){

    
}


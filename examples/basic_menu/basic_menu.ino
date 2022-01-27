

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


typedef enum{
    MAIN_MENU,
    SUB_MENU_1,
    SUB_MENU_2
} menu_dx_t;


int8_t cur_menu = MAIN_MENU;
int8_t prev_menu = -1;
int8_t prev_itm = -1;

const char      *menus[] = {mm_items[0], s1_items[0],s2_items[0]};
const uint8_t   menus_cnt[] = {mm_items_cnt,s1_cnt,s2_cnt};
const uint8_t   menus_len[] = {mm_items_len,s1_len,s2_len};


void setup(){
    Serial.begin(115200);
    Serial.println(F("program begin..."));

    u8x8_dev.setI2CAddress(0x78);
    u8x8_dev.begin();
    u8x8_dev.setFont(u8x8_font_chroma48medium8_r);
    disp.set_device(&u8x8_dev);
    /*
    Serial.print("disp.rows = ");
    Serial.println(disp.rows);
    Serial.print("disp.cols = ");
    Serial.println(disp.cols);
    */
#ifdef MNU_USE_ENCODER
    enc_setup();
#endif    

#ifdef MNU_USE_PB
    pb_setup();
#endif    
    
    my_menu.set_device(&disp);
    //my_menu.set_items(mm_items[0],mm_items_cnt,mm_items_len);
    //my_menu.set_title(F("My menu"));
    //my_menu.set_rows(4);
    my_menu.set_items(menus[MAIN_MENU],menus_cnt[MAIN_MENU],menus_len[MAIN_MENU]);
    my_menu.draw_menu();
    

}


void main_menu_actions(int8_t opt){
    switch(opt){
        case 0:
            //Serial.print(F("before disp_rows : "));
            //Serial.println(my_menu.disp_rows);
            if(my_menu.title == nullptr) my_menu.set_title(F("Main menu"));
            else my_menu.set_title();
            //Serial.print(F("after disp_rows : "));
            //Serial.println(my_menu.disp_rows);
            disp.clear_display();
            my_menu.draw_menu();
            break;
        case 1:
            my_menu.set_rows(2);
            disp.clear_display();
            my_menu.draw_menu();
            break;
        case 2:
            my_menu.set_rows(3);
            disp.clear_display();
            my_menu.draw_menu();
            break;
        case 3:
            my_menu.set_rows(4);
            disp.clear_display();
            my_menu.draw_menu();
            break;
        case 4:
            prev_itm = 4;
            prev_menu = MAIN_MENU;
            cur_menu = SUB_MENU_1;
            my_menu.set_items(menus[SUB_MENU_1],menus_cnt[SUB_MENU_1],menus_len[SUB_MENU_1]);
            //my_menu.cur_item = 0;
            my_menu.set_title();
            disp.clear_display();
            my_menu.draw_menu();
            break;
        case 5:
            //entering in sub-menu, save prev. menu and position so we ca restore later
            prev_itm = 5;
            prev_menu = MAIN_MENU;
            cur_menu = SUB_MENU_2;
            my_menu.set_items(menus[SUB_MENU_2],menus_cnt[SUB_MENU_2],menus_len[SUB_MENU_2]);
            //my_menu.cur_item = 0;
            my_menu.set_title();
            disp.clear_display();
            my_menu.draw_menu();
            break;

            
    }
}


void sub1_actions(int8_t opt){
    switch(opt){
        case 0:
            if(my_menu.title == nullptr) my_menu.set_title(F("Sub menu 1"));
            else my_menu.set_title();
            disp.clear_display();
            my_menu.draw_menu();
            break;
        case 5:
            //prev_itm = 5;
            //prev_menu = 0;
            cur_menu = prev_menu >= 0 ? prev_menu : MAIN_MENU;
            my_menu.set_items(menus[prev_menu],menus_cnt[prev_menu],menus_len[prev_menu]);
            my_menu.cur_item = prev_itm >= 0 ? prev_itm : 0;
            my_menu.set_title();
            //if(my_menu.cur_item > my_menu.disp_rows-1) my_menu.start_item = my_menu.cur_item - my_menu.disp_rows +1;
            disp.clear_display();
            my_menu.draw_menu();
            prev_itm = -1;
            prev_menu = -1;
            break;
    } 
}

void loop(){
    int8_t res = menu_loop(&my_menu);
    switch(cur_menu){
        case MAIN_MENU:
            main_menu_actions(res);
            break;
        case SUB_MENU_1:
            sub1_actions(res);
            break;

        
    }
    
}


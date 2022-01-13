
#include <basic_menu.h>

#ifdef EN_M_U8X8
#include <u8x8_display.h>
#endif



basic_menu::basic_menu(){
    device = nullptr;
    items.base = nullptr;
    items.cnt = 0;
    items.len = 0;
    disp_rows = 0;
    title = nullptr;
}


basic_menu::basic_menu(const char *base,uint8_t cnt,uint8_t item_l,uint8_t d_rows){
    device = nullptr;
    set_items(base,cnt,item_l);
    disp_rows = d_rows;
    title = nullptr;
}

basic_menu::basic_menu(text_display *dev,const char *base,uint8_t cnt,uint8_t item_l,uint8_t d_rows){
    device = dev;
    set_items(base,cnt,item_l);
    disp_rows = d_rows;
    title = nullptr;
    if(disp_rows == 0 and dev != nullptr) disp_rows = device->rows;
    
}

/*
basic_menu::basic_menu(uint8_t dev_type,void *dev){
    device = new_device(dev_type,dev);
    if(device != nullptr){
        device->get_display_data();
        if(disp_rows == 0 and device->rows > 0) disp_rows = device->rows;
        if(options & M_NEW_DEV == 0) options |= M_NEW_DEV;
    }
    cur_item = 0;
    start_item = 0;
    title = nullptr;
    items.base = nullptr;
    items.cnt = 0;
    items.len = 0;
}
*/

void basic_menu::set_device(text_display *dev){
    device = dev;
    if(disp_rows == 0 and dev != nullptr){
        device->get_display_data();
        disp_rows = device->rows;
    }
    
}

/*
void basic_menu::set_device(uint8_t dev_type,void *dev){
    device = new_device(dev_type,dev);
    if(device != nullptr){
        device->get_display_data();
        if(disp_rows == 0 and device->rows > 0) disp_rows = device->rows;
        if(options & M_NEW_DEV == 0)options |= M_NEW_DEV;
    }
}



text_display *basic_menu::new_device(uint8_t dev_type,void *dev){
    switch(dev_type){
        case M_CURSES:
#ifdef EN_M_CURSES
            return new curses_display(); //add PARAMETERS!!!
#else
            return new text_display();
#endif
            break;
        case M_U8G2:
#ifdef EN_M_U8G2
            return new curses_display(); //add PARAMETERS!!!
#else

            return new text_display();
#endif         
            break;
        case M_U8X8:
#ifdef EN_M_U8X8
            return new u8x8_display((U8X8 *)dev); 
#else
            return new text_display();
#endif            
            break;
            
        case M_LQ:
#ifdef EN_M_LQ
            return new lq_display((LiquidCrystal *)dev); 
#else
            return new text_display();
#endif            
            break;
       
        
    }
    return nullptr;
}
*/

void basic_menu::set_items(const char *base,uint8_t cnt,uint8_t item_l){
    items.base = base;
    items.cnt = cnt;
    items.len = item_l+1;
    cur_item = 0;
    start_item = 0;
}

void basic_menu::move_next(){
    cur_item ++;
    if(cur_item > items.cnt-1){
        if(options & M_NO_ROLLOVER){
            cur_item--;
            return;
        }
        else cur_item = 0;
    } 
    if(cur_item >= start_item and cur_item < start_item+disp_rows) {
        draw_menu();
        return;
    }
    if(cur_item+1 > disp_rows){
        start_item = cur_item - disp_rows + 1;
    }
    else start_item = 0;
    start_item = constrain(start_item,0,items.cnt-1);
    draw_menu();
}

void basic_menu::move_prev(){
    if(cur_item > 0) cur_item --;
    else {
        if(options & M_NO_ROLLOVER) return;
        else cur_item = items.cnt-1;
    }
    if(items.cnt <= disp_rows){
        draw_menu();
        return;
    }
    if(cur_item >= start_item and  cur_item - start_item < disp_rows-1) {
        draw_menu();
        return;
    }    
    if(cur_item+1 > disp_rows){
        if(start_item > 0) start_item--;
        else start_item = cur_item - disp_rows + 1;
    }
    else start_item = 0;
    start_item = constrain(start_item,0,items.cnt-1);
    draw_menu();
}

const char *basic_menu::get_row(uint8_t row){
    uint8_t r = constrain(row+start_item,0,items.cnt-1);
#if defined(ARDUINO_ARCH_AVR)
    memset(avr_buf,0,MENU_BUF_LEN);
    strlcpy_P(avr_buf,items[r],MENU_BUF_LEN);
    return avr_buf;
#else    
    return items[r];//base_addr+(this->item_len * (row+this->start_item));
#endif    
}

uint8_t basic_menu::get_cur_row(){
    return cur_item - start_item;
}

#if defined(ARDUINO_ARCH_AVR)
void basic_menu::set_title(const __FlashStringHelper *tit){
#else
void basic_menu::set_title(const char *tit){
#endif
    uint8_t i = 1;
    if(title != nullptr) i = 0;
    title = tit;
    if(disp_rows > 0 and title != nullptr) disp_rows -= i;
    if(i==0 and title==nullptr) disp_rows +=1;
}


void basic_menu::set_options(uint8_t opts, uint8_t set){
    if(set > 0) options |= opts;
    else options ^= opts;
}

void basic_menu::draw_menu(){
    uint8_t rev = 0;
    uint8_t r_cnt = min(items.cnt,disp_rows);//device->rows);
    uint8_t start = 0;
    
    if(device == nullptr) return;
    if(options & M_DRAW_CLEAR)device->clear_display();
    if(title != nullptr){
#if defined(ARDUINO_ARCH_AVR)
        memset(avr_buf,0,MENU_BUF_LEN);
        strlcpy_P(avr_buf,(const char*)title,MENU_BUF_LEN);
        device->print(0,(device->cols-strlen(avr_buf))/2,avr_buf);
#else        
        device->print(0,(device->cols-strlen(title))/2,title);
#endif
        start = 1;
    }
    for(uint8_t i = start;i<r_cnt+start;i++){
        if(i == get_cur_row()+start) rev = 1;
        else rev = 0;
        if(options | M_PRINT_CLEAR) device->clear_row(i,0);//,strlen(itm));//items.len);
        device->print(i,0,get_row(i-start),rev);
    }
    device->refresh();
}


#include <edit_menu.h>

//char  conv_buf[MENU_BUF_LEN];

uint8_t edit_item::set_next_digit(){
    if(edt_mode != EDT_MODE_DIGIT) return 0;
    switch(type){
        case EDT_NUMB:
            return set_next_digit_numb();
            break;
        case EDT_STRING:
            return set_next_char();
            break;
    }
    return 0;
}

uint8_t edit_item::set_next_digit_numb(){
    get_txt_value();
    if(cur_digit > 0) cur_digit--;
    else return 0;
    if(conv_buf[cur_digit] == '-') return 0;
    if(conv_buf[cur_digit] == '.'){
        if(cur_digit > 0) cur_digit--;
        else return 0;
    } 
    set_next_step();    
    return 1;
}

uint8_t edit_item::set_next_char(){
    get_txt_value();
    if(cur_digit < nmb_len-1) cur_digit++;
    else return 0;
    return 1;
}

/*******************************************************************************/


edit_ip::edit_ip(const char *itm_txt,uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4):edit_item(EDT_IP){
    value[3] = ip1;
    value[2] = ip2;
    value[1] = ip3;
    value[0] = ip4;
    txt = itm_txt;
    cur_oct = 0;
    cur_digit = 14;
    step = 1;
    n_type = NMB_UCHAR;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
    cur_fmt = ip_txt_fmt;
}

const char * edit_ip::get_txt_value(){
    memset(conv_buf,0,MENU_BUF_LEN);
    //if(fmt == 0) snprintf(conv_buf,MENU_BUF_LEN,ip_txt_fmt,value[3],value[2],value[1],value[0]);
    snprintf(conv_buf,MENU_BUF_LEN,cur_fmt,value[3],value[2],value[1],value[0]);
    return conv_buf;
}

uint8_t edit_ip::set_next_value(){
    value[cur_oct] = constrain(value[cur_oct]+step,0,255);
    if(value[cur_oct] == 255) return 1;
    return 0;
}

uint8_t edit_ip::set_prev_value(){
    value[cur_oct] = constrain(value[cur_oct]-step,0,255);
    if(value[cur_oct] == 0) return 1;
    return 0;
}

uint8_t edit_ip::set_next_digit(){
    
    if(cur_digit > 0) cur_digit--;
    else{
        cur_fmt = ip_txt_fmt;
        get_txt_value();
        cur_oct = 0;
        cur_digit = 14;
        return 0;
    }
    if(conv_buf[cur_digit] == ':'){
        cur_digit--;
        cur_oct++;
        reset_step();
    }
    else set_next_step();
    return 1;
}


/******************************************************************************/

edit_string::edit_string(const char *itm_txt,const char *str):edit_item(EDT_STRING){
    memset(conv_buf,0,MENU_BUF_LEN);
    strncpy(conv_buf,str,MENU_BUF_LEN-1);
    txt = itm_txt;
    cur_digit = 0;
    nmb_len = strlen(conv_buf);
    edt_mode = EDT_MODE_DIGIT;
}

void edit_string::strip(){
    nmb_len = strlen(conv_buf);
    while(conv_buf[0] == ' ') {
        memmove(conv_buf,&conv_buf[0],nmb_len);
        nmb_len--;
        cur_digit = 0;
    }
    while(conv_buf[nmb_len-1] == ' '){
        conv_buf[nmb_len-1] = '\0';
        nmb_len--;
        cur_digit = 0;
    }
}

uint8_t edit_string::set_next_value(){
    unsigned char c = conv_buf[cur_digit];
    c++;
    if(c == 127) c = 32;
    if(strchr(ban_chars,c) != nullptr) c++;
    //c = constrain(c,32,126);
    conv_buf[cur_digit] = c;
    strip();
    return 0;
};

uint8_t edit_string::set_prev_value(){
    unsigned char c = conv_buf[cur_digit];
    c--;
    if(c == 31) c = 126;
    if(strchr(ban_chars,c) != nullptr) c--;
    //c = constrain(c,32,126);
    conv_buf[cur_digit] = c;
    strip();
    return 0;
};





/******************************************************************************/

edit_menu::edit_menu(text_display *dev):basic_menu(dev,nullptr,0,0){
    disp_rows = device->rows;
    max_len = 0;
    state = 0;
}


void edit_menu::add_item(edit_item *itm){
    if(items.cnt < EDT_ITM_LEN) edt_items[items.cnt] = itm;
    else return;
    items.cnt ++;
    uint8_t l = strlen(edt_items[items.cnt-1]->txt);
    max_len = max(max_len,l);
}

const char *edit_menu::pad_txt(char *buf,const char *txt){
    uint8_t l = strlen(txt);
    memset(buf,0,10);
    memset(buf,' ',max_len-l);
    return buf;
}

const char *edit_menu::get_row(uint8_t row){
    uint8_t r = constrain(row+start_item,0,items.cnt-1);
    char pad_buf[10];
    memset(device->conv_buf,0,MENU_BUF_LEN);
    if(edt_items[r]->type == EDT_NONE) snprintf(device->conv_buf,MENU_BUF_LEN,"%s",edt_items[r]->txt);
    else snprintf(device->conv_buf,MENU_BUF_LEN,"%s%s - %s",edt_items[r]->txt,
                                                     pad_txt(pad_buf,edt_items[r]->txt),
                                                     edt_items[r]->get_txt_value());
    return device->conv_buf;
    
}

void edit_menu::edit_current_numb(uint8_t lmt){
    uint8_t start = (title == nullptr ? 0 : 1);
    char pad_buf[10];
    //if(edt_items[cur_item]->edt_mode > 0) edt_items[cur_item]->reset_step();
    memset(device->conv_buf,0,MENU_BUF_LEN);
    snprintf(device->conv_buf,MENU_BUF_LEN,"%s%s - ",edt_items[cur_item]->txt,
                                                    pad_txt(pad_buf,edt_items[cur_item]->txt));
    device->print(get_cur_row()+start,0,device->conv_buf,0);
    device->clear_row(get_cur_row()+start,max_len+3);
    if(edt_items[cur_item]->edt_mode == EDT_MODE_STEP){
        device->print(get_cur_row()+start,max_len+3,edt_items[cur_item]->get_txt_value(),1);
    }
    else{
        device->print(get_cur_row()+start,max_len+3,edt_items[cur_item]->get_txt_value());
        char b[2] = {'\0','\0'};
        b[0] = edt_items[cur_item]->get_txt_value()[edt_items[cur_item]->cur_digit];
        device->print(get_cur_row()+start,max_len+3+edt_items[cur_item]->cur_digit,b,2);
    }
    if(lmt > 0){
        if(lmt == 1){
            device->print(get_cur_row()+start,0,"!max!",1);
        }
        else device->print(get_cur_row()+start,0,"!min!",1);
    }
}


uint8_t edit_menu::edit_set_next_numb(){
    uint8_t l_bfr = strlen(edt_items[cur_item]->get_txt_value());
    uint8_t res = edt_items[cur_item]->set_next_value();
    uint8_t l_aft = strlen(edt_items[cur_item]->get_txt_value());
    if(edt_items[cur_item]->edt_mode > 0 and l_bfr != l_aft){
        edt_items[cur_item]->reset_step();
    }
    edit_current(res);
    return res;
}

uint8_t edit_menu::edit_set_prev_numb(){
    uint8_t l_bfr = strlen(edt_items[cur_item]->get_txt_value());
    uint8_t res = edt_items[cur_item]->set_prev_value();
    uint8_t l_aft = strlen(edt_items[cur_item]->get_txt_value());
    if(edt_items[cur_item]->edt_mode > 0 and l_bfr != l_aft){
        edt_items[cur_item]->reset_step();
    }
    //if(res == 1)
    edit_current(res);
    return res;
}

uint8_t edit_menu::get_cur_itm_mode(){
    return edt_items[cur_item]->edt_mode;
}

uint8_t edit_menu::set_next_digit(){
    uint8_t r = edt_items[cur_item]->set_next_digit();
    if(r == 0) state = EDT_STATE_MENU;
    return r;
}

uint8_t edit_menu::edit_set_next_list(){
    edt_items[cur_item]->set_next_value();
    edit_current();
    return 0;
}

uint8_t edit_menu::edit_set_prev_list(){
    edt_items[cur_item]->set_prev_value();
    edit_current();
    return 0;
}

void edit_menu::edit_current_list(){
    uint8_t start = (title == nullptr ? 0 : 1);
    if(state == 0){ 
        state = 1;
        //if(edt_items[cur_item]->edt_mode == 1)edt_items[cur_item]->reset_step();
    }
    char pad_buf[10];
    //if(edt_items[cur_item]->edt_mode > 0) edt_items[cur_item]->reset_step();
    memset(device->conv_buf,0,MENU_BUF_LEN);
    snprintf(device->conv_buf,MENU_BUF_LEN,"%s%s - ",edt_items[cur_item]->txt,
                                                    pad_txt(pad_buf,edt_items[cur_item]->txt));
    device->print(get_cur_row()+start,0,device->conv_buf,0);
    device->clear_row(get_cur_row()+start,max_len+3);
    //if(edt_items[cur_item]->edt_mode == 0){
    device->print(get_cur_row()+start,max_len+3,edt_items[cur_item]->get_txt_value(),1);
    //}
    /*
    else{
        device->print(get_cur_row()+start,max_len+3,edt_items[cur_item]->get_txt_value());
        char b[2] = {'\0','\0'};
        b[0] = edt_items[cur_item]->get_txt_value()[edt_items[cur_item]->cur_digit];
        device->print(get_cur_row()+start,max_len+3+edt_items[cur_item]->cur_digit,b,1);
    }
    if(lmt > 0){
        if(lmt == 1){
            device->print(get_cur_row()+start,0,"!max!",1);
        }
        else device->print(get_cur_row()+start,0,"!min!",1);
    }
    */
}


uint8_t edit_menu::edit_set_next(){
    switch(edt_items[cur_item]->type){
        case EDT_NUMB:
            return edit_set_next_numb();
            break;
        case EDT_LIST:
        case EDT_BOOL:
        case EDT_STRING:
        case EDT_IP:
            return edit_set_next_list();
            break;
    }
    return 0;
}

uint8_t edit_menu::edit_set_prev(){
    switch(edt_items[cur_item]->type){
        case EDT_NUMB:
            return edit_set_prev_numb();
            break;
        case EDT_LIST:
        case EDT_BOOL:
        case EDT_STRING:
        case EDT_IP:
            return edit_set_prev_list();
            break;
    }
    return 0;
}

void edit_menu::edit_current(uint8_t lmt){
    if(state == EDT_STATE_MENU){ 
        state = EDT_STATE_EDIT;
        if(edt_items[cur_item]->edt_mode == EDT_MODE_DIGIT)edt_items[cur_item]->reset_step();
    }
    
    switch(edt_items[cur_item]->type){
        case EDT_NUMB:
        case EDT_STRING:
        case EDT_IP:
            edit_current_numb(lmt);
            break;
        case EDT_LIST:
        case EDT_BOOL:
            edit_current_list();
            break;
        case EDT_NONE:
            return;
            break;
    }
    
}

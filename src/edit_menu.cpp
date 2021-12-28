#include <edit_menu.h>



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

template <typename T>
edit_numb<T>::edit_numb(uint8_t t,const char *text,T &val,const T min,const T max,const T stp):edit_item(EDT_NUMB){
    char b[2];
    value = val;
    if(min == 0 and max == 0){
        //min_val = std::numeric_limits<T>::min();
        //max_val = std::numeric_limits<T>::max();
        switch(t){
            case NMB_CHAR:  //int8_t
                min_val = INT8_MIN;
                max_val = INT8_MAX;
                break;
            case NMB_UCHAR: //uint8_t
                min_val = 0;
                max_val = UINT8_MAX;
                break;
            case NMB_S_INT:
            case NMB_INT:   // int16_t
                min_val = INT16_MIN;
                max_val = INT16_MAX;
                break;
            case NMB_S_UINT:
            case NMB_UINT:  //uint16_t
                min_val = 0;
                max_val = UINT16_MAX;
                break;
            case NMB_L_INT: //int32_t
                min_val = INT32_MIN;
                max_val = INT32_MAX;
                break;
            case NMB_L_UINT: //uint32_t
                min_val = 0;
                max_val = UINT32_MAX;
                break;
                
        }
    }
    else{
        min_val = min;
        max_val = max;
    }
    step = stp;
    //size = sizeof(T);
    //val_ptr = &value;
    n_type = t;
    txt = text;
    //T aaaa= std::numeric_limits<T>::min();
    nmb_len = strlen(get_txt_value());
    cur_digit = nmb_len -1;
    edt_mode = EDT_MODE_STEP;
}

template <typename T>
uint8_t edit_numb<T>::set_next_value(){
    value = constrain(value+step,min_val,max_val);
    if(value == max_val) return 1;
    return 0;
}

template <typename T>
uint8_t edit_numb<T>::set_prev_value(){
    value = constrain(value-step,min_val,max_val);
    if(value == min_val)return 2;
    return 0;
}

template <typename T>
void edit_numb<T>::reset_step(){
    if(n_type == NMB_FLOAT) step = .01;
    else step = 1;
    nmb_len = strlen(get_txt_value());
    cur_digit = nmb_len -1;
}

#if defined(ARDUINO_ARCH_AVR)
template <typename T>
void edit_numb<T>::strip(){
    nmb_len = strlen(conv_buf);
    uint8_t c = 0;
    while(conv_buf[c] == ' ') c++;
    if(c > 0){
        memmove(conv_buf,&conv_buf[c],nmb_len-c+1);
    }
    nmb_len = strlen(conv_buf);
    while(conv_buf[nmb_len-1] == ' '){
        conv_buf[nmb_len-1] = '\0';
        nmb_len--;
        //cur_digit = 0;
    }
}
#endif

template <typename T>
const char *edit_numb<T>::get_txt_value(){
    memset(conv_buf,0,MENU_BUF_LEN);
#if defined(ARDUINO_ARCH_AVR)
    if(n_type == NMB_FLOAT){
        dtostrf(value,MENU_BUF_LEN/2,2,conv_buf);
        strip();
    }
    else snprintf(conv_buf,MENU_BUF_LEN,nmb_fmt[n_type],value);
#else        
    snprintf(conv_buf,MENU_BUF_LEN,nmb_fmt[n_type],value);
#endif
    nmb_len = strlen(conv_buf);
    return conv_buf;
}

/*******************************************************************************/


edit_ip::edit_ip(const char *itm_txt,uint8_t &ip1,uint8_t &ip2,uint8_t &ip3,uint8_t &ip4):edit_item(EDT_IP){
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

#ifdef ARDUINO
edit_ip::edit_ip(const char *itm_txt,IPAddress ip):edit_item(EDT_IP){
    value[3] = ip[3];
    value[2] = ip[2];
    value[1] = ip[1];
    value[0] = ip[0];
    txt = itm_txt;
    cur_oct = 0;
    cur_digit = 14;
    step = 1;
    n_type = NMB_UCHAR;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
    cur_fmt = ip_txt_fmt;
}
#endif

const char * edit_ip::get_txt_value(){
    memset(conv_buf,0,MENU_BUF_LEN);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(conv_buf,MENU_BUF_LEN,cur_fmt,value[3],value[2],value[1],value[0]);
#else
    snprintf(conv_buf,MENU_BUF_LEN,cur_fmt,value[3],value[2],value[1],value[0]);
#endif    
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

/********************************************************************************/

edit_time::edit_time(const char *itm_txt,uint8_t &h,uint8_t &m,uint8_t &s):edit_item(EDT_TIME){
    value[TM_HOUR] = h;
    value[TM_MIN] = m;
    value[TM_SEC] = s;
    txt = itm_txt;
    cur_val = TM_SEC;
    cur_digit = time_fmt_len;
    step = 1;
    n_type = NMB_UCHAR;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
    
}

const char * edit_time::get_txt_value(){
    memset(conv_buf,0,MENU_BUF_LEN);
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(conv_buf,MENU_BUF_LEN,time_fmt,value[TM_HOUR],value[TM_MIN],value[TM_SEC]);
#else
    snprintf(conv_buf,MENU_BUF_LEN,time_fmt,value[TM_HOUR],value[TM_MIN],value[TM_SEC]);
#endif
    return conv_buf;
}

uint8_t edit_time::set_next_value(){
    uint8_t ret = 0;
    switch(cur_val){
        case TM_SEC:
        case TM_MIN:
            value[cur_val] = constrain(value[cur_val]+step,0,59);
            if(value[cur_val] == 59) ret = 1;
            break;
        case TM_HOUR:
            value[cur_val] = constrain(value[cur_val]+step,0,23);
            if(value[cur_val] == 23) ret = 1;
            break;
    }
    return ret;
}

uint8_t edit_time::set_prev_value(){
    uint8_t ret = 0;
    switch(cur_val){
        case TM_SEC:
        case TM_MIN:
            value[cur_val] = constrain(value[cur_val]-step,0,59);
            if(value[cur_val] == 0) ret = 1;
            break;
        case TM_HOUR:
            value[cur_val] = constrain(value[cur_val]-step,0,23);
            if(value[cur_val] == 0) ret = 1;
            break;
    }
    return ret;
}

uint8_t edit_time::set_next_digit(){
    
    if(cur_digit > 0) cur_digit--;
    else{
        //cur_fmt = ip_txt_fmt;
        get_txt_value();
        cur_val = 0;
        cur_digit = time_fmt_len;
        return 0;
    }
    if(conv_buf[cur_digit] == ':'){
        cur_digit--;
        cur_val--;
        reset_step();
    }
    else set_next_step();
    return 1;
}

/********************************************************************************/

edit_date::edit_date(const char *itm_txt,uint16_t &y,uint8_t &m,uint8_t &d):edit_item(EDT_DATE){
    value[DT_YEAR] = y;
    value[DT_MONTH] = m;
    value[DT_DAY] = d;
    txt = itm_txt;
    fmt = DT_YMD;
    sep = DT_SEP_SLASH;
    cur_val = DT_DAY;
    cur_digit = date_fmt_len;
    step = 1;
    n_type = NMB_UCHAR;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
    //uint8_t a = month_length(2021,2);
}


void  edit_date::set_fmt(uint8_t f){
    fmt = constrain(f,0,DT_FMT_CNT);
    switch(fmt){
        case DT_YMD:
            cur_val = DT_DAY;
            break;
        case DT_DMY:
        case DT_MDY:    
            cur_val = DT_YEAR;
            break;
    }
    
}


void  edit_date::set_sep(uint8_t s){
    sep = constrain(s,0,strlen(date_sep)-1);
}


const char * edit_date::get_txt_value(){
    memset(conv_buf,0,MENU_BUF_LEN);
    uint16_t a=0,b=0,c=0;
    switch(fmt){
        case DT_YMD:
            a = value[DT_YEAR];
            b = value[DT_MONTH];
            c = value[DT_DAY];
            break;
        case DT_DMY:
            a = value[DT_DAY];
            b = value[DT_MONTH];
            c = value[DT_YEAR];
            break;
        case DT_MDY:
            b = value[DT_DAY];
            a = value[DT_MONTH];
            c = value[DT_YEAR];
            break;
            
    }
#if defined(ARDUINO_ARCH_AVR)    
    snprintf_P(conv_buf,MENU_BUF_LEN,date_fmt[fmt],a,date_sep[sep],b,date_sep[sep],c);
#else
    snprintf(conv_buf,MENU_BUF_LEN,date_fmt[fmt],a,date_sep[sep],b,date_sep[sep],c);
#endif
    return conv_buf;
}

uint8_t edit_date::set_next_digit(){
    
    if(cur_digit > 0) cur_digit--;
    else{
        //cur_fmt = ip_txt_fmt;
        get_txt_value();
        //cur_val = 0;
        switch(fmt){
            case DT_YMD:
                cur_val = DT_DAY;
                break;
            case DT_DMY:
            case DT_MDY:    
                cur_val = DT_YEAR;
                break;
        }
        
        cur_digit = date_fmt_len;
        return 0;
    }
    if(conv_buf[cur_digit] == date_sep[sep]){
        cur_digit--;
        //cur_val--;
        switch(cur_val){
            case DT_YEAR:
                switch(fmt){
                    case DT_DMY:
                        cur_val = DT_MONTH;
                        break;
                    case DT_MDY:
                        cur_val = DT_DAY;
                        break;
                }
                break;
            case DT_MONTH:
                switch(fmt){
                    case DT_YMD:
                        cur_val = DT_YEAR;
                        break;
                    case DT_DMY:
                        cur_val = DT_DAY;
                        break;
                }
                break;
            case DT_DAY:
                cur_val = DT_MONTH;
                break;
        }
        reset_step();
    }
    else set_next_step();
    return 1;
}


uint8_t edit_date::set_next_value(){
    uint8_t ret = 0;
    switch(cur_val){
        case DT_DAY:
            value[cur_val] = constrain(value[cur_val]+step,1,month_length(value[DT_YEAR],value[DT_MONTH]));
            if(value[cur_val] == month_length(value[DT_YEAR],value[DT_MONTH])) ret = 1;
            break;
        case DT_MONTH:
            value[cur_val] = constrain(value[cur_val]+step,1,12);
            if(value[cur_val] == 12) ret = 1;
            break;
        case DT_YEAR:
            value[cur_val] = constrain(value[cur_val]+step,DT_YEAR_MIN,DT_YEAR_MAX);
            if(value[cur_val] == DT_YEAR_MAX) ret = 1;
            break;
    }
    return ret;
}


uint8_t edit_date::set_prev_value(){
    uint8_t ret = 0;
    switch(cur_val){
        case DT_DAY:
            value[cur_val] = constrain(value[cur_val]-step,1,month_length(value[DT_YEAR],value[DT_MONTH]));
            if(value[cur_val] == 1) ret = 1;
            break;
        case DT_MONTH:
            value[cur_val] = constrain(value[cur_val]-step,1,12);
            if(value[cur_val] == 1) ret = 1;
            break;
        case DT_YEAR:
            value[cur_val] = constrain(value[cur_val]-step,DT_YEAR_MIN,DT_YEAR_MAX);
            if(value[cur_val] == DT_YEAR_MIN) ret = 1;
            break;
    }
    return ret;
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
        case EDT_TIME:
        case EDT_DATE:
        case EDT_NUMB_LIST:
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
        case EDT_TIME:
        case EDT_DATE:
        case EDT_NUMB_LIST:
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
        case EDT_TIME:
        case EDT_DATE:
            edit_current_numb(lmt);
            break;
        case EDT_LIST:
        case EDT_BOOL:
        case EDT_NUMB_LIST:
            edit_current_list();
            break;
        case EDT_NONE:
            return;
            break;
    }
    
}

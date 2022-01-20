#include <edit_menu.h>


void edit_item::set_options(uint8_t opts,uint8_t set){
    if(set > 0) options |= opts;
    else //options ^= opts;    
    {
        if(options & opts) options -= opts;
    }
}

const char * edit_item::get_menu_line(uint8_t max_len,uint8_t lmt,uint8_t txt_only){
    char val[MENU_BUF_LEN];
    
    memset(val,0,MENU_BUF_LEN);
    strncpy(val,get_txt_value(),MENU_BUF_LEN-1);
    uint8_t l = 0;
    memset(conv_buf,0,MENU_BUF_LEN);
#if defined(ARDUINO_ARCH_AVR)
    l = strlen_P(txt);
    memcpy_P(conv_buf,txt,l);
#else
    l = strlen(txt);
    memcpy(conv_buf,txt,l);
#endif
    for(uint8_t i=l;i<max_len+1;i++) memset(conv_buf+i,' ',1); //padding with space
    memset(conv_buf+max_len+1,'-',1);
    memset(conv_buf+max_len+2,' ',1);
    if(txt_only > 0) return conv_buf;
    memccpy(conv_buf+max_len+3,val,0,lmt-(max_len+3));
    return conv_buf;
}


void edit_item::edit(text_display *device, uint8_t row, uint8_t col,uint8_t max_len,uint8_t no_txt){
    uint8_t av_cols = min(MENU_BUF_LEN,device->cols);
    uint8_t val_len = strlen(get_txt_value());
    uint8_t pr_col = av_cols-val_len-1;
    device->clear_row(row,0);
    if(no_txt == 0){
        uint8_t txt_len = strlen(get_menu_line(max_len,av_cols,1));
        if(txt_len > av_cols-val_len-2) conv_buf[av_cols-val_len-3]=0;
        device->print(row,col,conv_buf);
    }
    else pr_col = col;
    if(edt_mode == EDT_MODE_STEP){
        device->print(row,pr_col,get_txt_value(),1);
    }
    else{
        device->print(row,pr_col,get_txt_value());
        char b[2] = {'\0','\0'};
        b[0] = get_txt_value()[cur_digit];
        device->print(row,pr_col+cur_digit,b,2);
    }
    
    
}

/*******************************************************************************/

/*
*/



template <typename T>
edit_numb<T>::edit_numb(uint8_t t):edit_item(EDT_NUMB){
    n_type = t;
    //init(0);
    value = 0;
    //step = 1;
    options = 0;
    reset_step();
}


template <typename T>
edit_numb<T>::edit_numb(uint8_t t,T &val):edit_item(EDT_NUMB){
    n_type = t;
    init(val);
}


template <typename T>
void edit_numb<T>::init(T &val,const T min,const T max,const T stp){
    value = val;
    options = 0;
    if(min == 0 and max == 0){
        min_val = 0;
        max_val = 0;
        set_options(NMB_CHK_LOW,0);
        set_options(NMB_CHK_HIGH,0);
    }
    else{
        min_val = min;
        set_options(NMB_CHK_LOW);
        if(max > min){
            max_val = max;
            set_options(NMB_CHK_HIGH);
        }
    }
    step = stp; //TODO FIX this
    reset_step();
}


template <typename T>
void edit_numb<T>::set_min(const T min){
    min_val = min;
    set_options(NMB_CHK_LOW);
}

template <typename T>
void edit_numb<T>::set_max(const T max){
    max_val = max;
    set_options(NMB_CHK_HIGH);
}


template <typename T>
const char *edit_numb<T>::get_txt_value(){
    memset(conv_buf,0,MENU_BUF_LEN);
#if defined(ARDUINO_ARCH_AVR)
    if(n_type == NMB_FLOAT){
        dtostrf(value,0-(MENU_BUF_LEN-1),2,conv_buf);
        for(uint8_t i=0;i<MENU_BUF_LEN;i++){
            if(conv_buf[i] == ' '){
                conv_buf[i] = 0;
                break;
            }
        }
    }
    else snprintf(conv_buf,MENU_BUF_LEN,nmb_fmt[n_type],value);
#else        
    snprintf(conv_buf,MENU_BUF_LEN,nmb_fmt[n_type],value);
#endif
    nmb_len = strlen(conv_buf);
    return conv_buf;
}

template <typename T>
uint8_t edit_numb<T>::set_next_value(){
    uint8_t ret = 0;
    uint8_t l_bfr = strlen(get_txt_value());
    if((options & NMB_CHK_HIGH) == 0){
        value += step;
        ret = 0;
    }
    else{
        value = constrain(value+step,min_val,max_val); //this is safe (min_val 0 as default)
        if(value == max_val) ret = 1;
    }
    if(edt_mode == EDT_MODE_DIGIT and l_bfr != strlen(get_txt_value())) reset_step();
    return ret;
}

template <typename T>
uint8_t edit_numb<T>::set_prev_value(){
    uint8_t ret = 0;
    uint8_t l_bfr = strlen(get_txt_value());
    if((options & NMB_CHK_LOW) == 0){
        value -= step;
        ret = 0;
    }
    else{
        if(max_val > min_val) value = constrain(value-step,min_val,max_val); //this must be checked (if max_val is not set)
        else{
            if(value - step < min_val) value = min_val;
            else value -= step;
        }
        if(value == min_val)ret = 2;
     }
    if(edt_mode == EDT_MODE_DIGIT and l_bfr != strlen(get_txt_value())) reset_step();
    return ret;
}

template <typename T>
void edit_numb<T>::reset_step(){
    if(n_type == NMB_FLOAT) step = .01;
    else step = 1;
    get_txt_value();
    cur_digit = nmb_len -1;
}



template <typename T>
uint8_t edit_numb<T>::set_next_digit(){
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

template <typename T>
uint8_t edit_numb<T>::set_prev_digit(){
    get_txt_value();
    if(cur_digit < nmb_len-1) cur_digit++;
    else return 0;
    if(conv_buf[cur_digit] == '.'){
        if(cur_digit < nmb_len-1) cur_digit++;
        else return 0;
    } 
    set_prev_step();    
    return 1;
}

template class edit_numb<char>;
template class edit_numb<unsigned char>;
template class edit_numb<short int>;
template class edit_numb<unsigned short int>;
template class edit_numb<int>;
template class edit_numb<unsigned int>;
template class edit_numb<long>;
template class edit_numb<unsigned long>;
template class edit_numb<float>;

/*******************************************************************************/

edit_ip::edit_ip(uint8_t &ip1,uint8_t &ip2,uint8_t &ip3,uint8_t &ip4):edit_item(EDT_IP){
    init(ip1,ip2,ip3,ip4);
}

edit_ip::edit_ip(IPAddress ip):edit_item(EDT_IP){
    init(ip[3],ip[2],ip[1],ip[0]);
}

void edit_ip::init(IPAddress ip){
    init(ip[3],ip[2],ip[1],ip[0]);
}

void edit_ip::init(uint8_t &ip1,uint8_t &ip2,uint8_t &ip3,uint8_t &ip4){
    value[3] = ip1;
    value[2] = ip2;
    value[1] = ip3;
    value[0] = ip4;
    cur_oct = 0;
    cur_digit = IP_FMT_LEN;
    step = 1;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
    cur_fmt = ip_txt_fmt;
}


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
        cur_digit = IP_FMT_LEN;
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

uint8_t edit_ip::set_prev_digit(){
    if(cur_digit < IP_FMT_LEN) cur_digit++;
    else{
        cur_fmt = ip_txt_fmt;
        get_txt_value();
        cur_oct = 0;
        cur_digit = IP_FMT_LEN;
        return 0;
    }
    if(conv_buf[cur_digit] == ':'){
        cur_digit++;
        cur_oct--;
        //reset_step();
        step = 100;
    }
    else set_prev_step();
    return 1;
}

/********************************************************************************/


edit_time::edit_time(uint8_t &h,uint8_t &m,uint8_t &s):edit_item(EDT_TIME){
    init(h,m,s);
}

edit_time::edit_time():edit_item(EDT_TIME){
    value[TM_HOUR] = 0;
    value[TM_MIN] = 0;
    value[TM_SEC] = 0;
    cur_val = TM_SEC;
    cur_digit = TIME_FMT_LEN;
    step = 1;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
}

void edit_time::init(uint8_t &h,uint8_t &m,uint8_t &s){
    value[TM_HOUR] = h;
    value[TM_MIN] = m;
    value[TM_SEC] = s;
    cur_val = TM_SEC;
    cur_digit = TIME_FMT_LEN;
    step = 1;
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
        get_txt_value();
        cur_val = TM_SEC;
        cur_digit = TIME_FMT_LEN;
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

uint8_t edit_time::set_prev_digit(){
    if(cur_digit < TIME_FMT_LEN) cur_digit++;
    else{
        get_txt_value();
        cur_val = TM_SEC;
        cur_digit = TIME_FMT_LEN;
        return 0;
    }
    if(conv_buf[cur_digit] == ':'){
        cur_digit++;
        cur_val++;
        step = 10;
    }
    else set_prev_step();
    return 1;
}



/********************************************************************************/


edit_date::edit_date(uint16_t &y,uint8_t &m,uint8_t &d):edit_item(EDT_DATE){
    init(y,m,d);
}


edit_date::edit_date():edit_item(EDT_DATE){
    //init(DT_YEAR_MIN,1,1);
    value[DT_YEAR] = DT_YEAR_MIN;
    value[DT_MONTH] = 1;
    value[DT_DAY] = 1;
    fmt = DT_YMD;
    sep = DT_SEP_SLASH;
    cur_val = DT_DAY;
    cur_digit = DATE_FMT_LEN;
    step = 1;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
}


void edit_date::init(uint16_t &y,uint8_t &m,uint8_t &d){
    value[DT_YEAR] = y;
    value[DT_MONTH] = m;
    value[DT_DAY] = d;
    fmt = DT_YMD;
    sep = DT_SEP_SLASH;
    cur_val = DT_DAY;
    cur_digit = DATE_FMT_LEN;
    step = 1;
    edt_mode = EDT_MODE_DIGIT;
    memset(conv_buf,0,MENU_BUF_LEN);
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
        get_txt_value();
        switch(fmt){
            case DT_YMD:
                cur_val = DT_DAY;
                break;
            case DT_DMY:
            case DT_MDY:    
                cur_val = DT_YEAR;
                break;
        }
        
        cur_digit = DATE_FMT_LEN;
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


uint8_t edit_date::set_prev_digit(){
    
    if(cur_digit < DATE_FMT_LEN) cur_digit++;
    else{
        get_txt_value();
        switch(fmt){
            case DT_YMD:
                cur_val = DT_DAY;
                break;
            case DT_DMY:
            case DT_MDY:    
                cur_val = DT_YEAR;
                break;
        }
        
        cur_digit = DATE_FMT_LEN;
        return 0;
    }
    if(conv_buf[cur_digit] == date_sep[sep]){
        cur_digit++;
        switch(cur_val){
            case DT_YEAR:
                cur_val = DT_MONTH;
                step = 10;
                break;
            case DT_MONTH:
                switch(fmt){
                    case DT_YMD:
                    case DT_MDY:
                        cur_val = DT_DAY;
                        step = 10;
                        break;
                    case DT_DMY:
                        cur_val = DT_YEAR;
                        step = 1000;
                        break;
                }
                break;
            case DT_DAY:
                if(fmt == DT_DMY){ 
                    cur_val = DT_MONTH;
                    step = 10;
                }
                else{
                    cur_val = DT_YEAR;
                    step = 1000;
                }
                break;
        }
    }
    else set_prev_step();
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

edit_string::edit_string(const char *str):edit_item(EDT_STRING){
    init(str);
}

edit_string::edit_string():edit_item(EDT_STRING){
    init((const char*)nullptr);
}

#if defined(ARDUINO_ARCH_AVR)
void edit_string::init(const __FlashStringHelper *str){
    memset(conv_buf,0,MENU_BUF_LEN);
    if(str != nullptr) strncpy_P(conv_buf,(const char*)str,MENU_BUF_LEN-1);
    cur_digit = 0;
    nmb_len = strlen(conv_buf);
    edt_mode = EDT_MODE_DIGIT;
}
#endif

void edit_string::init(const char *str){
    memset(conv_buf,0,MENU_BUF_LEN);
    if(str != nullptr) strncpy(conv_buf,str,MENU_BUF_LEN-1);
    cur_digit = 0;
    nmb_len = strlen(conv_buf);
    edt_mode = EDT_MODE_DIGIT;
}

void edit_string::init(uint8_t l){
    uint8_t m = constrain(l,1,MENU_BUF_LEN-1);
    memset(conv_buf,0,MENU_BUF_LEN);
    memset(conv_buf,' ',m);
    cur_digit = 0;
    nmb_len = strlen(conv_buf);
    edt_mode = EDT_MODE_DIGIT;
    set_options(STR_NO_STRP_L);
    set_options(STR_NO_STRP_T);
}


void edit_string::strip(){
    nmb_len = strlen(conv_buf);
    if(options & STR_NO_STRP_L == 0){
        while(conv_buf[0] == ' ') {
            memmove(conv_buf,&conv_buf[0],nmb_len);
            nmb_len--;
            cur_digit = 0;
        }
    }
    if(options & STR_NO_STRP_T == 0){    
        while(conv_buf[nmb_len-1] == ' '){
            conv_buf[nmb_len-1] = '\0';
            nmb_len--;
            cur_digit = 0;
        }
    }
}

uint8_t edit_string::set_next_value(){
    unsigned char c = conv_buf[cur_digit];
    c++;
    if(c == 127) c = 32;
    if(strchr(ban_chars,c) != nullptr) c++;
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


uint8_t edit_string::set_next_digit(){
    get_txt_value();
    if(cur_digit < nmb_len-1) cur_digit++;
    else return 0;
    return 1;
};

uint8_t edit_string::set_prev_digit(){
    get_txt_value();
    if(cur_digit > 0) cur_digit--;
    else return 0;
    return 1;
};


const char * edit_string::get_txt_value(){
    nmb_len = strlen(conv_buf);
    return conv_buf;
}

/*******************************************************************************/

edit_list::edit_list(const char *base,uint8_t cnt,uint8_t item_l):edit_item(EDT_LIST){
    init(base,cnt,item_l);
}

edit_list::edit_list():edit_item(EDT_LIST){
    init(nullptr,0,0);
}

void edit_list::init(const char *base,uint8_t cnt,uint8_t item_l){    
    edt_mode = EDT_MODE_STEP;
    set_items(base,cnt,item_l);
    disp_rows = 1;
    device = nullptr;
}

uint8_t edit_list::set_next_value(){
    move_prev();
    return 0;
}

uint8_t edit_list::set_prev_value(){
    move_next();
    return 0;
}

const char *edit_list::get_txt_value(){
#if defined(ARDUINO_ARCH_AVR)
    memset(avr_buf,0,MENU_BUF_LEN);
    strlcpy_P(avr_buf,items[cur_item],MENU_BUF_LEN);
    return  avr_buf;
#else        
    return items[cur_item];
#endif
}

/*******************************************************************************/

edit_bool::edit_bool(bool &val):edit_item(EDT_BOOL){
    value = val;
    style = SB_AST;
}

edit_bool::edit_bool():edit_item(EDT_BOOL){
    value = false;
    style = SB_AST;
}

void edit_bool::set_style(uint8_t s){
    s = constrain(s,0,BOOL_CNT-1);
    style = s;
}

uint8_t edit_bool::set_next_value(){
    value = !value;
    return 0;
}

uint8_t edit_bool::set_prev_value(){
    value = !value;
    return 0;
}

const char *edit_bool::get_txt_value(){
#if defined(ARDUINO_ARCH_AVR)
    memset(conv_buf,0,MENU_BUF_LEN);
    if(value) strlcpy_P(conv_buf,true_symbols[style],MENU_BUF_LEN-1);
    else strlcpy_P(conv_buf,false_symbols[style],MENU_BUF_LEN-1);
    return conv_buf;
#else        
    if (value) return true_symbols[style];
    else return false_symbols[style];
#endif        
}




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

void edit_menu::add_item(edit_item *itm,const char *text){
    if(items.cnt < EDT_ITM_LEN) edt_items[items.cnt] = itm;
    else return;
    itm->set_txt(text);
    items.cnt ++;
    uint8_t l = strlen(edt_items[items.cnt-1]->txt);
    max_len = max(max_len,l);
}

#if defined(ARDUINO_ARCH_AVR)
void edit_menu::add_item(edit_item *itm,const __FlashStringHelper *text){
    if(items.cnt < EDT_ITM_LEN) edt_items[items.cnt] = itm;
    else return;
    itm->set_txt(text);
    items.cnt ++;
    uint8_t l = strlen_P((const char *)text);
    max_len = max(max_len,l);
}
#endif



const char *edit_menu::get_row(uint8_t row){
    uint8_t r = constrain(row+start_item,0,items.cnt-1);
    uint8_t lmt = min(MENU_BUF_LEN,device->cols);
    return edt_items[r]->get_menu_line(max_len,lmt);
}




uint8_t edit_menu::set_next_digit(){
    uint8_t r = edt_items[cur_item]->set_next_digit();
    if(r == 0) state = EDT_STATE_MENU;
    return r;
}

uint8_t edit_menu::set_prev_digit(){
    uint8_t r = edt_items[cur_item]->set_prev_digit();
    if(r == 0) state = EDT_STATE_MENU;
    return r;
}


uint8_t edit_menu::edit_set_next(){
    uint8_t res = edt_items[cur_item]->set_next_value();
    edit_current(res);
    return res;
}

uint8_t edit_menu::edit_set_prev(){
    uint8_t res = edt_items[cur_item]->set_prev_value();
    edit_current(res);
    return res;
}

void edit_menu::edit_current(uint8_t lmt){
    if(state == EDT_STATE_MENU){ 
        state = EDT_STATE_EDIT;
        if(edt_items[cur_item]->edt_mode == EDT_MODE_DIGIT)edt_items[cur_item]->reset_step();
    }
    
    edt_items[cur_item]->edit(device,get_cur_row()+(title == nullptr ? 0 : 1) ,0,max_len);
}

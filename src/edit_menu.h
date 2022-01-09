#ifndef __EDIT_MENU_H__

#define __EDIT_MENU_H__

#include <basic_menu.h>
#include <time.h>


#ifdef ARDUINO
#include <IPAddress.h>
#endif

#define EDT_ITM_LEN 10

//AVR libc++ does not have <limits> header, will test other platform
//#include <limits>

typedef enum{
    EDT_NUMB,
    EDT_STRING,
    EDT_BOOL,
    EDT_LIST,
    EDT_IP,
    EDT_TIME,
    EDT_DATE,
    EDT_NUMB_LIST,
    EDT_NONE    // just menu item, no edit, used for exit,save,etc
} edt_type_t;

typedef enum{
    NMB_CHAR,   //signed char
    NMB_UCHAR,  //unsigned char
    NMB_S_INT,  //short int
    NMB_S_UINT, //unsigned short int
    NMB_INT,    //int
    NMB_UINT,   //unsigned int
    NMB_L_INT,  //long
    NMB_L_UINT, //unsigned long
    NMB_FLOAT,  //float
    //NMB_DOUBLE, //double
    NMB_CNT     //total
} nmb_type_t;


//AVR libc already define some macros for this, but is it portable?
const char nmb_fmt[NMB_CNT][5] = { "%i","%u","%hi","%hu","%i","%u","%li","%lu","%.2f"/*,"%.2g"*/};

//int imin = std::numeric_limits<int>::min();

typedef enum{
    EDT_MODE_STEP,
    EDT_MODE_DIGIT,
    EDT_MODE_NONE
} edt_mode_t;


typedef enum{
    EDT_STATE_MENU,
    EDT_STATE_EDIT
} edt_state_t;



class edit_item{
public:
    uint8_t     type;       //edt_type_t
    
    
    const char  *txt;
    char        conv_buf[MENU_BUF_LEN];
    uint8_t     cur_digit;
    uint8_t     nmb_len;
    uint8_t     edt_mode; // 0 - by step +-  1 - by digit 2 - no edit
    
    edit_item(edt_type_t t, const char *text){type=t;txt=text;};
    
    
    virtual uint8_t set_next_digit(){return 0;};
    
    void set_edit_mode(uint8_t m){edt_mode = m;};
    virtual uint8_t set_next_value(){return 0;};
    virtual uint8_t set_prev_value(){return 0;};
    virtual void set_next_step(){};
    virtual void set_prev_step(){};
    virtual void reset_step(){};
    virtual void edit(text_display *disp, uint8_t row=0, uint8_t col=0, uint8_t rows=0){};
    virtual const char  *get_txt_value(){return nullptr;};
    const char          *get_menu_line(uint8_t max_len,uint8_t lmt,uint8_t txt_only=0);
/*    
private:
    uint8_t set_next_digit_numb();
    uint8_t set_next_char();
*/
    
};

template <typename T>
class edit_numb:public edit_item{
public:
    
    
    T               min_val;
    T               max_val;
    T               value;
    T               step;
    uint8_t         n_type;     //nmb_type_t

#if defined(ARDUINO_ARCH_AVR)    
    edit_numb(uint8_t t,const __FlashStringHelper *text,T &val,const T min=0,const T max=0,const T stp=1);
#else
    edit_numb(uint8_t t,const char *text,T &val,const T min,const T max,const T stp);
#endif    

    T get_value(){return value;};
    
    uint8_t set_next_value();
    uint8_t set_prev_value();
    uint8_t set_next_digit();
    void set_step(T stp){step=stp;};
    void set_next_step(){step *=10;};
    void set_prev_step(){step /=10;};
    void reset_step();
    void edit(text_display *disp, uint8_t row=0, uint8_t col=0, uint8_t rows=0);
#if defined(ARDUINO_ARCH_AVR)
    //void strip();
#endif    
    const char *get_txt_value();
};


template <typename T>
#if defined(ARDUINO_ARCH_AVR)    
edit_numb<T>::edit_numb(uint8_t t,const __FlashStringHelper *text,T &val,const T min,const T max,const T stp):edit_item(EDT_NUMB,(const char*)text)
#else
edit_numb<T>::edit_numb(uint8_t t,const char *text,T &val,const T min,const T max,const T stp):edit_item(EDT_NUMB,text)
#endif
{
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
    //txt = text;
    //T aaaa= std::numeric_limits<T>::min();
    nmb_len = strlen(get_txt_value());
    cur_digit = nmb_len -1;
    edt_mode = EDT_MODE_STEP;
}



template <typename T>
const char *edit_numb<T>::get_txt_value(){
    memset(conv_buf,0,MENU_BUF_LEN);
    //uint8_t
#if defined(ARDUINO_ARCH_AVR)
    if(n_type == NMB_FLOAT){
        dtostrf(value,0-(MENU_BUF_LEN-1),2,conv_buf);
        for(uint8_t i=0;i<MENU_BUF_LEN;i++){
            if(conv_buf[i] == ' '){
                conv_buf[i] = 0;
                break;
            }
        }
        //strip();
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

/*
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
*/

template <typename T>
void edit_numb<T>::edit(text_display *disp, uint8_t row, uint8_t col, uint8_t rows){
    memset(conv_buf,0,MENU_BUF_LEN);
    snprintf(conv_buf,MENU_BUF_LEN,"%s [%li / %li]",txt,min_val,max_val);
    disp->clear_row(row,col,strlen(conv_buf));
    disp->print(row,col,conv_buf);
    disp->clear_row(row+1,col);
    disp->print(row+1,col,get_txt_value());
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



/*
class edit_none:public edit_item{
public:
    
    edit_none(const char*text):edit_item(EDT_NONE){
        txt = text;
        edt_mode = EDT_MODE_NONE;
    };
};
*/

#if defined(ARDUINO_ARCH_AVR) 
const PROGMEM char      ip_txt_fmt[] = "%3u:%3u:%3u:%3u";
const PROGMEM char      ip_edt_fmt[] = "%03u:%03u:%03u:%03u";
#else
const char      ip_txt_fmt[] = "%3u:%3u:%3u:%3u";
const char      ip_edt_fmt[] = "%03u:%03u:%03u:%03u";
#endif
const uint8_t   ip_fmt_len = 14;


class edit_ip:public edit_item{
public:
    
    uint8_t         value[4];
    uint8_t         cur_oct;
    uint8_t         step;
    const char      *cur_fmt;
#if defined(ARDUINO_ARCH_AVR)
    edit_ip(const __FlashStringHelper *itm_txt,uint8_t &ip1,uint8_t &ip2,uint8_t &ip3,uint8_t &ip4);
    edit_ip(const __FlashStringHelper *itm_txt,IPAddress ip);
#else
    edit_ip(const char *itm_txt,uint8_t &ip1,uint8_t &ip2,uint8_t &ip3,uint8_t &ip4);
    edit_ip(const char *itm_txt,IPAddress ip);
#endif
    
    const char  *get_txt_value();
    uint8_t     set_next_value();
    uint8_t     set_prev_value();
    void        set_next_step(){step *=10;};
    void        set_prev_step(){step /=10;};
    void        reset_step(){step = 1;cur_fmt = ip_edt_fmt;};
    uint8_t     set_next_digit();
    
};


typedef enum{
    TM_HOUR,
    TM_MIN,
    TM_SEC
} tm_ndx_t;

#if defined(ARDUINO_ARCH_AVR)     
const PROGMEM char  time_fmt[] = "%02u:%02u:%02u";
#else
const char  time_fmt[] = "%02u:%02u:%02u";
#endif
const uint8_t time_fmt_len = 7;

class edit_time:public edit_item{
public:
    uint8_t     value[3];
    uint8_t     cur_val;
    uint8_t     step;

#if defined(ARDUINO_ARCH_AVR)
    edit_time(const __FlashStringHelper *itm_txt,uint8_t &h,uint8_t &m,uint8_t &s);
#else    
    edit_time(const char *itm_txt,uint8_t &h,uint8_t &m,uint8_t &s);
#endif
    const char  *get_txt_value();
    uint8_t     set_next_value();
    uint8_t     set_prev_value();
    void        set_next_step(){step *=10;};
    void        set_prev_step(){step /=10;};
    void        reset_step(){step = 1;};
    uint8_t     set_next_digit();
    
};


typedef enum{
    DT_YEAR,
    DT_MONTH,
    DT_DAY
} dt_ndx_t;

typedef enum{
    DT_YMD,
    DT_DMY,
    DT_MDY,
    DT_FMT_CNT
} dt_fmt_t;

typedef enum{
    DT_SEP_SLASH,
    DT_SEP_BSLASH,
    DT_SEP_MINUS,
    DT_SEP_COLON,
    DT_SEP_CNT
} dt_sep_t;

const char date_sep[] = "/\\-;:";


const uint8_t date_fmt_len = 9;

#if defined(ARDUINO_ARCH_AVR)
const PROGMEM char  date_fmt[][16] = {"%4u%c%02u%c%02u",
                              "%02u%c%02u%c%4u",
                              "%02u%c%02u%c%4u"
};
#else
const char  date_fmt[][16] = {"%4u%c%02u%c%02u",
                              "%02u%c%02u%c%4u",
                              "%02u%c%02u%c%4u"
};

#endif

#define DT_YEAR_MIN     1900
#define DT_YEAR_MAX     2999

class edit_date:public edit_item{
public:
    uint16_t    value[3];
    uint8_t     cur_val;
    uint8_t     step;
    uint8_t     fmt;
    uint8_t     sep;
    
#if defined(ARDUINO_ARCH_AVR)
    edit_date(const __FlashStringHelper *itm_txt,uint16_t &y,uint8_t &m,uint8_t &d);
#else
    edit_date(const char *itm_txt,uint16_t &y,uint8_t &m,uint8_t &d);
#endif
    const char  *get_txt_value();
    uint8_t     set_next_value();
    uint8_t     set_prev_value();
    void        set_next_step(){step *=10;};
    void        set_prev_step(){step /=10;};
    void        reset_step(){step = 1;};
    uint8_t     set_next_digit();
    void        set_fmt(uint8_t f);
    void        set_sep(uint8_t s);
    
};


const char ban_chars[] = "\"%";

class edit_string:public edit_item{
public:
    
#if defined(ARDUINO_ARCH_AVR)
    edit_string(const __FlashStringHelper *itm_txt,const char *str);
#else
    edit_string(const char *itm_txt,const char *str);
#endif    
    void strip();
    
    uint8_t set_next_value();
    uint8_t set_prev_value();
    uint8_t set_next_digit(){
        get_txt_value();
        if(cur_digit < nmb_len-1) cur_digit++;
        else return 0;
        return 1;
    };
    
    const char *get_txt_value(){
        nmb_len = strlen(conv_buf);
        return conv_buf;
    }
    
    void reset_step(){
        cur_digit = 0;
    }
    
};

/* Numeric list, only integer supported, no float!!
 * 
 */
template <typename T>
class edit_numb_list:public edit_item,public basic_menu{
public:
    const T     *base;
    uint8_t     cnt;
    uint8_t     size;

#if defined(ARDUINO_ARCH_AVR)
    edit_numb_list(const __FlashStringHelper *text,const T *b,uint8_t cnt):edit_item(EDT_NUMB_LIST,(const char*)text)
#else
    edit_numb_list(const char *text,const T *b,uint8_t cnt):edit_item(EDT_NUMB_LIST,text)
#endif
    {
        edt_mode = EDT_MODE_STEP;
        set_items(nullptr,cnt,0);
        disp_rows = 1;
        device = nullptr;
        size = sizeof(T);
        base = b;
    }

    uint8_t set_next_value(){
        move_prev();
        return 0;
    }

    uint8_t set_prev_value(){
        move_next();
        return 0;
    }
    
    const char *get_txt_value(){
#if defined(ARDUINO_ARCH_AVR)
        memset(avr_buf,0,MENU_BUF_LEN);
        switch(size){
            case 1:
                itoa(pgm_read_byte(base+cur_item),avr_buf,10);
                break;
            case 2:
                itoa(pgm_read_word(base+cur_item),avr_buf,10);
                break;
            case 4:
                ltoa(pgm_read_dword(base+cur_item),avr_buf,10);
                break;
                
        }
        //strlcpy_P(avr_buf,items[cur_item],MENU_BUF_LEN);
        return  avr_buf;
#else        
        snprintf(conv_buf,MENU_BUF_LEN,"%li",*(base+cur_item))
        return conv_buf;
#endif
    }
};


class edit_list:public edit_item,public basic_menu{
public:
#if defined(ARDUINO_ARCH_AVR)
    edit_list(const __FlashStringHelper *text,const char *base,uint8_t cnt,uint8_t item_l);
#else
    edit_list(const char *text,const char *base,uint8_t cnt,uint8_t item_l);
#endif
    uint8_t set_next_value();
    uint8_t set_prev_value();
    const char *get_txt_value();
    
};


#if defined(ARDUINO_ARCH_AVR)
const PROGMEM char true_symbols[3][4] = {"ON ","YES","[*]"};
const PROGMEM char false_symbols[3][4] = {"OFF","NO ","[ ]"};
#else
const char true_symbols[3][4] = {"ON ","YES","[*]"};
const char false_symbols[3][4] = {"OFF","NO ","[ ]"};
#endif

typedef enum{
    ON_OFF,
    YES_NO,
    SB_AST,
    BOOL_CNT
} bool_txt_t;

class edit_bool:public edit_item{
public:
    bool    value;
    uint8_t style;
    
#if defined(ARDUINO_ARCH_AVR)
    edit_bool(const __FlashStringHelper *text,bool &val);
#else
    edit_bool(const char *text,bool &val);
#endif
    void set_style(uint8_t s);
    uint8_t set_next_value();
    uint8_t set_prev_value();
    const char *get_txt_value();
    
};


class edit_menu:public basic_menu{
public:
    edit_item   *edt_items[EDT_ITM_LEN];
    uint8_t     max_len;
    uint8_t     state; // 0-menu   1-edit
    
    
    edit_menu(text_display *dev);
    void            add_item(edit_item *itm);
    //const char      *pad_txt(char *buf,const char *txt);
    void            edit_current(uint8_t lmt=0);  //0- OK  1- max_val hitted  2- min_val hitted
    uint8_t         edit_set_next();
    uint8_t         edit_set_prev();
    uint8_t         get_cur_itm_mode();
    uint8_t         set_next_digit();
    const char      *get_row(uint8_t row);

private:
    uint8_t         edit_set_next_numb();
    uint8_t         edit_set_prev_numb();
    void            edit_current_numb(uint8_t lmt=0);  //0- OK  1- max_val hitted  2- min_val hitted    
    void            edit_current_list();
    uint8_t         edit_set_next_list();
    uint8_t         edit_set_prev_list();
    
    
};





#endif

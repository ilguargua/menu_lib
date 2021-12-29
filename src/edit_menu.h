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


//char        conv_buf[MENU_BUF_LEN] = {0};

class edit_item{
public:
    uint8_t     type;       //edt_type_t
    uint8_t     n_type;     //nmb_type_t
    
    const char  *txt;
    char        conv_buf[MENU_BUF_LEN];
    uint8_t     cur_digit;
    uint8_t     nmb_len;
    uint8_t     edt_mode; // 0 - by step +-  1 - by digit 2 - no edit
    
    edit_item(edt_type_t t){type=t;};
    
    
    virtual uint8_t set_next_digit();
    
    void set_edit_mode(uint8_t m){edt_mode = m;};
    virtual uint8_t set_next_value(){return 0;};
    virtual uint8_t set_prev_value(){return 0;};
    virtual void set_next_step(){};
    virtual void set_prev_step(){};
    virtual void reset_step(){};
    virtual void edit(text_display *disp, uint8_t row=0, uint8_t col=0, uint8_t rows=0){};
    virtual const char *get_txt_value(){return nullptr;};
private:
    uint8_t set_next_digit_numb();
    uint8_t set_next_char();
};

template <typename T>
class edit_numb:public edit_item{
public:
    
    
    T               min_val;
    T               max_val;
    T               value;
    T               step;
    
    edit_numb(uint8_t t,const char *text,T &val,const T min=0,const T max=0,const T stp=1);

    T get_value(){return value;};
    
    uint8_t set_next_value();
    uint8_t set_prev_value();
    void set_step(T stp){step=stp;};
    void set_next_step(){step *=10;};
    void set_prev_step(){step /=10;};
    void reset_step();
    void edit(text_display *disp, uint8_t row=0, uint8_t col=0, uint8_t rows=0);
#if defined(ARDUINO_ARCH_AVR)
    void strip();
#endif    
    const char *get_txt_value();
};


class edit_none:public edit_item{
public:
    
    edit_none(const char*text):edit_item(EDT_NONE){
        txt = text;
        edt_mode = EDT_MODE_NONE;
    };
};


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

    edit_ip(const char *itm_txt,uint8_t &ip1,uint8_t &ip2,uint8_t &ip3,uint8_t &ip4);//:edit_item(EDT_IP);
#ifdef ARDUINO
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
    
    edit_time(const char *itm_txt,uint8_t &h,uint8_t &m,uint8_t &s);

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
    
    edit_date(const char *itm_txt,uint16_t &y,uint8_t &m,uint8_t &d);

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
    
    
    edit_string(const char *itm_txt,const char *str);//:edit_item(EDT_STRING){
    
    void strip();
    
    uint8_t set_next_value();
    
    uint8_t set_prev_value();
    
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
    
    edit_numb_list(const char *text,const T *b,uint8_t cnt):edit_item(EDT_NUMB_LIST){
        txt = text;
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

    edit_list(const char *text,const char *base,uint8_t cnt,uint8_t item_l):edit_item(EDT_LIST)
    {    
        txt = text;
        edt_mode = EDT_MODE_STEP;
        set_items(base,cnt,item_l);
        disp_rows = 1;
        device = nullptr;
    };
    
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
        strlcpy_P(avr_buf,items[cur_item],MENU_BUF_LEN);
        return  avr_buf;
#else        
        return items[cur_item];
#endif
    }
    
};



const char true_symbols[3][4] = {"ON ","YES","[*]"};
const char false_symbols[3][4] = {"OFF","NO ","[ ]"};

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
    
    edit_bool(const char *text,bool &val):edit_item(EDT_BOOL){
        value = val;
        txt = text;
        style = SB_AST;
    };
    
    void set_style(uint8_t s){
        s = constrain(s,0,BOOL_CNT-1);
        style = s;
    }
    
    uint8_t set_next_value(){
        value = !value;
        return 0;
    }

    uint8_t set_prev_value(){
        value = !value;
        return 0;
    }
    
    const char *get_txt_value(){
        if (value) return true_symbols[style];
        else return false_symbols[style];
    }
    
};


class edit_menu:public basic_menu{
public:
    edit_item   *edt_items[EDT_ITM_LEN];
    uint8_t     max_len;
    uint8_t     state; // 0-menu   1-edit
    
    
    edit_menu(text_display *dev);
    void            add_item(edit_item *itm);
    const char      *pad_txt(char *buf,const char *txt);
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

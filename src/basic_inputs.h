#ifndef __BASIC_INPUTS_H__

#define __BASIC_INPUTS_H__


#define MAX_PBTNS   5       //max buttons for collections
#define DEB_TIME    100     //debounce time in mS


class basic_input{
public:
    
    uint8_t     pin;        //button pin
    int8_t      active;     //status when active, HIGH or LOW
    int8_t      id;         //value returned for valid button push
    int8_t      empty_val;  //value returned when no button pushed es. 0 or -1
    
    basic_input(uint8_t p, uint8_t act,uint8_t val){
        pin = p;
        active = act;
        id = val;
        empty_val = -1;
    };
    
    void set_empty(int8_t v){empty_val = v;};
    
    int8_t read_pin(){
        int8_t st = digitalRead(pin);
        return (st == active) ? id : empty_val;
    };

};


class timed_input:public basic_input{
public:
    
    uint32_t      t_rise;         //reading time
    
    push_button(uint8_t p, uint8_t act,uint8_t val):basic_input(p,act,val){
        //ps = digitalRead(pin);
        //t = millis();
        t_rise = 0;
    };
    

    int8_t read_pin(){
        int8_t st = digitalRead(pin);
        t_rise = millis();
        if(st == active){
            t_rise = millis();
            return id;
        } 
        else return empty_val;
    };
    
    uint32_t get_time(){return t_rise};
};


/*
 * menu_button
 * A input class suitable for menus
 * Button id is returned when button is released
 * 
 */
class menu_button:public basic_input{
public:
    
    int8_t      ps;         //previous status
    
    menu_button(uint8_t p, uint8_t act,uint8_t val):basic_input(p,act,val){
        ps = digitalRead(pin);
    };
    
    int8_t get_status(){
        if(digitalRead(pin) == active){
            if(ps == !active) ps = active;
            return empty_val;
        }
        else{
            if(ps == active){
                ps = !active;
                return id;
            }
        }
        return empty_val;
    };

};


class debounce_button:public timed_input{
public:
    
    int8_t      ps;         //previous status
    
    debounce_button(uint8_t p, uint8_t act,uint8_t val):basic_input(p,act,val){
        ps = digitalRead(pin);
    };
    
    int8_t get_status(){
        if(get_time()+DEB_TIME > millis()) return empty_val;
        if(read_pin() == id){
            if(ps == !active){
              ps = active;
              return empty_val;
            } 
            else return id;
        }
        return empty_val;
    };

};



class pb_collection{
public:
    
    menu_button     *btns[MAX_PBTNS];
    uint8_t         cnt;
    int8_t          empty_val;
    
    pb_collection(){cnt=0;empty_val=0;};
    
    
    void add_btn(push_button *btn){
        btns[cnt] = btn;
        if(cnt < MAX_PBTNS-1) cnt++;
    };
    
    void add_btn(uint8_t p, uint8_t act,uint8_t val){
        add_btn(new menu_button(p,act,val));
    };
    int8_t get_btns_status(){
        int8_t r = 0;
        for(uint8_t i=0;i < cnt;i++){
            r = btns[i]->get_status();
            if(r != empty_val) return r;
        }
        return empty_val;
    };
    
    void set_empty(int8_t v){
        empty_val = v;
        for(uint8_t i=0;i < cnt;i++){
            btns[i]->set_empty(v);
        }
    };
    
};



#endif

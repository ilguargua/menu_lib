/*
simple test to check a Nextion interface
this file is part of menu_lib Arduino library (https://github.com/ilguargua/menu_lib)

This skecth is a just a test of a display functionality, and can be used to calculate font size.

2022 - Alessandro Guarguaglini - ilguargua@gmail.com
*/




#include <SoftwareSerial.h>
#include <nextion_display.h>


#define DRAW_LINE_FMT   PSTR("line %i,%i,%i,%i,65504%c%c%c")   
#define CMD_BUF_LEN     60

//Font size, in pixel
//It must be adjusted for the font in use
//This value works fine for ms sans serif size 16
//For best results use mono spaced font (eg. courier new)
#define FONT_W          12
#define FONT_H          14


#define RLR_PTC         (FONT_W / 2)

//Serial interface for nextion connection
//Can be any valid hardware/software serial port
SoftwareSerial intf(10,11);  //we use software serial for display, so Serial can be used for debug
                             //In this case: blu wire to pin 10, yellow wire to pin 11 


//text_display instance,will be initialized in setup
nextion_display disp;



void setup(){
    //buffer used for various commands
    char buf[CMD_BUF_LEN];
    memset(buf,0,CMD_BUF_LEN);
    
    //Nextion serial port init
    intf.begin(38400);  //This have to match the nextion baudrate
    
    //If we need for debug
    Serial.begin(115200);
    Serial.println("Program begin..");
    
    //text_display init, parameters are: serial port,rows,cols,font index,font height,font width
    disp.init(&intf,10,25,1,FONT_H,FONT_W);
    
    //use this if the font is a proportional font (not monospace)
    //this is mandatory for editing, not needed in menu, and is much slower to update display
    disp.set_options(NXT_PROP_FONT);
    
    //we reset the display, useful for testing connection
    snprintf_P(buf,CMD_BUF_LEN,PSTR("rest%c%c%c"),NXT_MSG_END);
    intf.print(buf);
    delay(1000);
    
    uint16_t x_st = disp.menu_x+2;
    uint16_t y_st = disp.menu_y+2;
    uint16_t line_end = x_st+(FONT_W * 22);
    
    //we init the display area
    disp.clear_display();
    
    //print something on screen (in reverse)
    disp.print(0,0,"Hello world 1234567890",1);

    //draw a ruler on screen, useful to understand font size
    //short pitch are at font width/2, long pitch at font width
    memset(buf,0,CMD_BUF_LEN);
    snprintf_P(buf,CMD_BUF_LEN,DRAW_LINE_FMT,x_st,y_st+16,line_end,y_st+16,NXT_MSG_END);
    intf.print(buf);
    uint8_t ofs = 35;
    for(uint16_t i=x_st;i<line_end+1;i+=RLR_PTC){
        memset(buf,0,CMD_BUF_LEN);
        snprintf_P(buf,CMD_BUF_LEN,DRAW_LINE_FMT,i,disp.menu_y+18,i,disp.menu_y+ofs,NXT_MSG_END);
        intf.print(buf);
        //delay(50);
        if(ofs == 35) ofs = 25;
        else ofs = 35;
    }
    memset(buf,0,CMD_BUF_LEN);
    snprintf_P(buf,CMD_BUF_LEN,DRAW_LINE_FMT,x_st,disp.menu_y+35,line_end,disp.menu_y+35,NXT_MSG_END);
    intf.print(buf);
    
    //other test lines
    disp.print(3,0,"Highlite txt line",1);
    disp.print(4,0,"Normal text line");
    disp.print(5,0,"Hilite digit 123456");
    disp.print(5,15,"3",1);
    disp.print(6,0,"01234567890123456",1);

}



void loop(){
    
}



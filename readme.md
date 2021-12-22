# Menu_lib, an Arduino menu library

**Please note that the library is actually a work in progress, no docs or comments yet**

A menu library for Arduino subsystem, aimed at small monochromatic display that can be used with (at least)
3 pushbutton or a rotary encoder (with pushbutton).

## How does it work?

A **basic_menu** class do almost of the job, taking as parameter an array of string (char array). To actual display
on screen a virtual class, **text_display**, is used. **text_display** supply few methods for printing or clearing
a row on the display. Is subclassed by  interfaces classes for various kind of display, at the moment LiquidCrystal,
U8X8,Nextion, but other are planned or can be easy implemented.

## Editing
Along with the simple menu, a **menu_edit** class can help to modify value of many kind of variables. 
As today, the following editing classes are defined:

- **edit_numb<T>** for numeric values (specifing <T> as numeric type, es uint8_t)
- **edit_string** for editing a simple char array
- **edit_bool** for boolean values
- **edit_list** for choosing a value from a list of string
- **edit_ip** for IP values (IP v4 only)
- **edit_time** for editing time (HH:MM:SS)
- **edit_date** for dates editing in various format (es. YYYY/MM/DD)



(c) Alessandro Guarguaglini - ilguargua@gmail.com - 2021

# Menu_lib, an Arduino menu library

**Please note that the library is actually a work in progress, no docs or comments yet**

A menu library for Arduino subsystem, aimed at small monochromatic display. You can then interact with it using
(at least)3 pushbutton or a rotary encoder (with pushbutton).

## How does it work?

A **basic_menu** class do almost of the job, taking as parameter an array of string (char array). To actual display
on screen a virtual class, **text_display**, is used. **text_display** supply few methods for printing or clearing
a row on the display. **text_display** is used as a base class for other specialized classes that can drive the actual
display.
At the moment, the following interfaces are supported:

- [**LiquidCrystal**](https://www.arduino.cc/en/Reference/LiquidCrystal), the standard Arduino lib for alphanumeric display
- [**U8x8**](https://github.com/olikraus/u8g2), the text-only part of U8g2, for used the small I2C/SPI Oled display.
- [**Nextion**](https://nextion.tech/instruction-set/), not tested yet. It does not require any changes to the Nextion firmware

Other interfaces are planned, or can be easy implemented.

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

The examples are messy and incomplete, since I'm still testing new feature.

**Please be aware that the code in this repository is not (and probably never will) complete or usable.** 


(c) Alessandro Guarguaglini - ilguargua@gmail.com - 2021

# DS1307_RTC
DS1307 Real time clock library for the ATTiny85 (ATTinyX5).

Features:

*   system time support, with external reference available
*   time library patched for **time drift correction**
*   system time clock even when the micro is the sleep period (POWER_DOWN) and the internal millis() timer is off (with RTC)
*   changes to save space for ATTiny85 micro. 



_The sensor actually communicate with micro via I2C protocol: the implementation supported is only TinyWireM [http://playground.arduino.cc/Code/USIi2c] , to save code space on the ATTiny MCU._    

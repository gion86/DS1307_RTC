// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!
// Modified to use a private: USI_TWI &busI2C reference at the I2C bus
// from the application. Static method removed to use the class members.

#include <TinyWireM.h>
#include <Time.h>

#define DS1307_ADDRESS 0x68

/**
 * RTC based on the DS1307 chip connected via I2C and the Wire library
 */
class DS1307RTC {
public:
  DS1307RTC(USI_TWI &bus);

  uint8_t isrunning(void);
  void sqw(int sqw);

  time_t get();
  void set(const time_t& t);
  void read(tmElements_t &tm);
  void write(tmElements_t &tm);
  tmDriftInfo read_DriftInfo();         // new function not in original DS1307RC.h
  void write_DriftInfo(tmDriftInfo di); // new function not in original DS1307RC.h

private:
  static uint8_t dec2bcd(uint8_t num);
  static uint8_t bcd2dec(uint8_t num);

  USI_TWI &busI2C;
};


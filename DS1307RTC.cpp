// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!
// Modified to use a private: USI_TWI &busI2C reference at the I2C bus
// from the application. Static method removed to use the class members.
// .......

#include <DS1307RTC.h>
#include <TinyWireM.h>

RTC_DS1307::RTC_DS1307(USI_TWI &bus) :
    busI2C(bus) {
}

// ##############################################################################
// PUBLIC FUNCTIONS

uint8_t RTC_DS1307::isrunning(void) {
  busI2C.beginTransmission(DS1307_ADDRESS);
  busI2C.send(0x00);
  busI2C.endTransmission();

  busI2C.requestFrom(DS1307_ADDRESS, 1);
  uint8_t ss = busI2C.receive();
  return !(ss >> 7);
}

void RTC_DS1307::sqw(int sqw) {
  busI2C.beginTransmission(DS1307_ADDRESS);
  busI2C.send(0x07);
  switch (sqw) {
    case 0:
      busI2C.send(0x80); // Off
      break;
    case 1:
      busI2C.send(0x10); // Frequency 1Hz
      break;
    case 2:
      busI2C.send(0x11); // Frequency 4096kHz
      break;
    case 3:
      busI2C.send(0x12); // Frequency 8192kHz
      break;
    case 4:
      busI2C.send(0x13); // Frequency 32768kHz
      break;
  }
  busI2C.endTransmission();
}

time_t RTC_DS1307::get() // Acquire data from buffer and convert to time_t
{
  tmElements_t tm;
  read(tm);
  return (makeTime(tm));
}

void RTC_DS1307::set(const time_t &t) {
  tmElements_t tm;
  breakTime(t, tm);
  tm.Second |= 0x80; // stop the clock
  write(tm);
  tm.Second &= 0x7f; // start the clock
  write(tm);
}

// Aquire data from the RTC chip in BCD format
void RTC_DS1307::read(tmElements_t &tm) {
  busI2C.beginTransmission(DS1307_ADDRESS);
  busI2C.send(0x00);
  busI2C.endTransmission();

  // request the 7 data fields   (secs, min, hr, dow, date, mth, yr)
  busI2C.requestFrom(DS1307_ADDRESS, tmNbrFields);
  tm.Second = bcd2dec(busI2C.receive() & 0x7f);
  tm.Minute = bcd2dec(busI2C.receive());
  tm.Hour = bcd2dec(busI2C.receive() & 0x3f); // mask assumes 24hr clock
  tm.Wday = bcd2dec(busI2C.receive());
  tm.Day = bcd2dec(busI2C.receive());
  tm.Month = bcd2dec(busI2C.receive());
  tm.Year = y2kYearToTm((bcd2dec(busI2C.receive())));
}

void RTC_DS1307::write(tmElements_t &tm) {
  busI2C.beginTransmission(DS1307_ADDRESS);

  busI2C.send(0x00); // reset register pointer
  busI2C.send(dec2bcd(tm.Second));
  busI2C.send(dec2bcd(tm.Minute));
  busI2C.send(dec2bcd(tm.Hour)); // sets 24 hour format
  busI2C.send(dec2bcd(tm.Wday));
  busI2C.send(dec2bcd(tm.Day));
  busI2C.send(dec2bcd(tm.Month));
  busI2C.send(dec2bcd(tmYearToY2k(tm.Year)));
  busI2C.endTransmission();
}

// ##############################################################################
// new function not in original DS1307RC.cpp
tmDriftInfo RTC_DS1307::read_DriftInfo() {
  tmDriftInfo t;
  t.DriftStart = 0;
  t.DriftDays = 0;
  t.DriftSeconds = 0;

  busI2C.beginTransmission(DS1307_ADDRESS);

  busI2C.send(0x08);
  busI2C.endTransmission();

  busI2C.requestFrom(DS1307_ADDRESS, 8);

  t.DriftStart = busI2C.receive();
  t.DriftStart = t.DriftStart << 8;
  t.DriftStart = t.DriftStart | busI2C.receive();
  t.DriftStart = t.DriftStart << 8;
  t.DriftStart = t.DriftStart | busI2C.receive();
  t.DriftStart = t.DriftStart << 8;
  t.DriftStart = t.DriftStart | busI2C.receive();

  t.DriftDays = t.DriftDays | busI2C.receive();
  t.DriftDays = t.DriftDays << 8;
  t.DriftDays = t.DriftDays | busI2C.receive();

  t.DriftSeconds = t.DriftSeconds | busI2C.receive();
  t.DriftSeconds = t.DriftSeconds << 8;
  t.DriftSeconds = t.DriftSeconds | busI2C.receive();
  return t;
}

// ##############################################################################
// new function not in original DS1307RC.cpp
void RTC_DS1307::write_DriftInfo(tmDriftInfo di) {
  busI2C.beginTransmission(DS1307_ADDRESS);

  busI2C.send(0x08); // set register pointer
  busI2C.send((di.DriftStart >> 24) & 0xff);
  busI2C.send((di.DriftStart >> 16) & 0xff);
  busI2C.send((di.DriftStart >> 8) & 0xff);
  busI2C.send(di.DriftStart & 0xff);

  busI2C.send((di.DriftDays >> 8) & 0xff);
  busI2C.send(di.DriftDays & 0xff);

  busI2C.send((di.DriftSeconds >> 8) & 0xff);
  busI2C.send(di.DriftSeconds & 0xff);

  busI2C.endTransmission();
}

// ##############################################################################
// PRIVATE FUNCTIONS

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t RTC_DS1307::dec2bcd(uint8_t num) {
  return ((num / 10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t RTC_DS1307::bcd2dec(uint8_t num) {
  return ((num / 16 * 10) + (num % 16));
}


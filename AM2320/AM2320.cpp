#include "AM2320.h"
#include <Wire.h>
//
// AM2321 Temperature & Humidity Sensor library for Arduino
// inspirated and some code based on https://github.com/thakshak/AM2320 

unsigned int CRC16(byte *ptr, byte length)
{
  unsigned int crc = 0xFFFF;
  uint8_t s = 0x00;

  while (length--) {
    crc ^= *ptr++;
    for (s = 0; s < 8; s++) {
      if ((crc & 0x01) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      } else crc >>= 1;
    }
  }
  return crc;
}

AM2320::AM2320()
{
}

//******************************************************************************************
int AM2320::rawread(int start, int size)
{
  if (size>sizeof(buf)) {
	  return -3; // Buffer too small
  }
	
  for (int s = 0; s < sizeof(buf); s++) buf[s] = 0x00;

  Wire.beginTransmission(AM2320_address);
  Wire.endTransmission(1);
  delay(1);
  Wire.beginTransmission(AM2320_address);
  Wire.write(0x03);
  Wire.write(start);
  Wire.write(size);
  if (Wire.endTransmission(1) != 0) return -1;
  delay(2); //>1.5ms
  //
  Wire.requestFrom(AM2320_address, 0x04+size); // 2 byte header, n byte data, 2 byte crc 
  for (int i = 0; i < 0x04+size; i++) {
	  buf[i] = Wire.read();
  }

  // CRC check
  unsigned int Rcrc = (buf[0x04+size-1] << 8) + (buf[0x04+size-2]);
  if (Rcrc == CRC16(buf, 0x04+size-2)) {
    return 0;
  }

  return -4;
}

//******************************************************************************************
int AM2320::Read()
{
  int res = rawread(0x00,0x04);
  if (res == 0) {
    unsigned int temperature = ((buf[4] & 0x7F) << 8) + buf[5];
    t = temperature / 10.0;
    t = ((buf[4] & 0x80) >> 7) == 1 ? t * (-1) : t;

    unsigned int humidity = (buf[2] << 8) + buf[3];
    h = humidity / 10.0;
    return 0;
  }
  
  return res;
}

//******************************************************************************************
unsigned int AM2320::DeviceID(void) {
  int res = rawread(0x0B,0x04);
  if (res == 0) {
	return (buf[2] << 24) + (buf[3] << 16) + (buf[4] << 8) + (buf[5]);
  }
  
  return abs(res);
}

int AM2320::Model(void) {
  int res = rawread(0x08,0x02);
  if (res == 0) {
	return (buf[2] << 8) + (buf[3]);
  }
  
  return res;
}







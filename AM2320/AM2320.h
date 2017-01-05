#ifndef AM2320_H
#define AM2320_H

#include <Arduino.h>

#define AM2320_address (0xB8 >> 1) 

class AM2320
{
	public:
		AM2320();
		float t;
		float h;
		/*
		  Error codes: 
		    -4: CRC error
			     Solution: try again. if error persists, check i2c bus
			-3: buffer too small (should not happen)
			-1: wake up fails or no sensor
			     Solution: try again. if error persists, verify sensor exists/check wiring
		*/
		int Read(void); 
		
		// This is a bit hacky: lower than 10 is probably an error code, assuming there are no serial that low
		// on the other hand: all my devices only returned 0 as Id and Model (with valid crc), 
		// so this feature of the am2320 does not work at all
		unsigned int DeviceID(void);
		// same error codes as Read() 		
		int Model(void);
    private:
    	byte buf[12];
		int rawread(int start, int size);
};

#endif

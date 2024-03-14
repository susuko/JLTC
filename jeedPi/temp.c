#include <wiringPiI2C.h>
#include <stdint.h>
#include "jeedPi.h"

#define ADT7410_REG 0x00

double readTemp(int fd)
{
	uint16_t data = wiringPiI2CReadReg16(fd, ADT7410_REG);
	return ((int16_t)(data << 8 | data >> 8) >> 3) / 16.0;
}

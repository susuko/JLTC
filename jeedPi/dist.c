#include <stdint.h>
#include <wiringPiI2C.h>
#include "jeedPi.h"

#define GP2Y0E03_REG_DIST_U8 0x5E

uint8_t readDist(int fd)
{
	return (uint8_t)wiringPiI2CReadReg8(fd, GP2Y0E03_REG_DIST_U8) >> 2;
}

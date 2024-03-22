#include <stdint.h>
#include <wiringPiI2C.h>
#include "jeedPi.h"

#define GP2Y0E03_REG_DIST_U8 0x5E
#define GP2Y0E03_REG_DIST_L4 0x5F

double readDist(int fd)
{
	uint16_t dist =
		wiringPiI2CReadReg8(fd, GP2Y0E03_REG_DIST_U8) << 4 |
		wiringPiI2CReadReg8(fd, GP2Y0E03_REG_DIST_L4);
	return dist / 64.0;
}

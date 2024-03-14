#include <stdint.h>
#include <wiringPiSPI.h>
#include "jeedPi.h"

uint16_t readAdc(int spi_ch, uint8_t adc_ch)
{
	const uint8_t start = 0x4, single = 0x2;
	uint8_t d0 = adc_ch >> 0 & 1, d1 = adc_ch >> 1 & 1, d2 = adc_ch >> 2 & 1;
	uint8_t bytes[3] = { start | single | d2, d1 << 7 | d0 << 6, 0 };
	
	wiringPiSPIDataRW(spi_ch, bytes, sizeof(bytes));
	
	return (bytes[1] & 0xF) << 8 | bytes[2];
}

#include <stdint.h>
#include <wiringPiSPI.h>
#include "jeedPi.h"

typedef union {
	struct {
		uint8_t lower_half;
		uint8_t upper_half;
	};
	struct {
		uint16_t data : 12;
		uint8_t shdn : 1;
		uint8_t gain : 1;
		uint8_t buff : 1;
		uint8_t ch_b : 1;
	};
} t_dac;

void writeDac(int spi_ch, uint8_t dac_ch, uint16_t data)
{
	t_dac dac = {
		.data = data, .shdn = 1, .gain = 1, .buff = 0, .ch_b = dac_ch
	};
	wiringPiSPIDataRW(spi_ch, (uint8_t[]) {
		dac.upper_half, dac.lower_half
	}, sizeof(dac));
}

#ifndef _JEED_PI_H
#define _JEED_PI_H

#include <stdint.h>

// AQM1602
#define AQM1602_ADR 0x3E

void initLcd(int fd);
void clearLcd(int fd);
void newlineLcd(int fd);
void putcLcd(int fd, char c);
void putsLcd(int fd, char *s);
void setCursorLcd(int fd, int _line_number, int _cursor);
void setCgRamLcd(int fd, uint8_t address, uint8_t *data);

// ADT7410
#define ADT7410_ADR 0x48

double readTemp(int fd);

// GP2Y0E03
#define GP2Y0E03_ADR 0x40

uint8_t readDist(int fd);

// MCP4922
void writeDac(int spi_ch, uint8_t dac_ch, uint16_t data);

// MCP3208
uint16_t readAdc(int spi_ch, uint8_t adc_ch);

#endif

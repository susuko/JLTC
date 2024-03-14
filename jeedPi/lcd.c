#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include "jeedPi.h"

// RS Bit
#define AQM1602_RS0 0b00000000
#define AQM1602_RS1 0b01000000

// Instruction Code
#define AQM1602_IST(db7, db6, db5, db4, db3, db2, db1, db0) ( \
	db7 << 7 | db6 << 6 | db5 << 5 | db4 << 4 | \
	db3 << 3 | db2 << 2 | db1 << 1 | db0 << 0 \
)

// Global Instructions
#define AQM1602_CLR_DISP() \
	AQM1602_IST(0, 0, 0, 0, 0, 0, 0, 1)
#define AQM1602_RET_HOME() \
	AQM1602_IST(0, 0, 0, 0, 0, 0, 1, 0)
#define AQM1602_SET_ENTR(id, s) \
	AQM1602_IST(0, 0, 0, 0, 0, 1, id, s)
#define AQM1602_SET_DISP(d, c, b) \
	AQM1602_IST(0, 0, 0, 0, 1, d, c, b)
#define AQM1602_SET_FUNC(dl, n, dh, is) \
	AQM1602_IST(0, 0, 1, dl, n, dh, 0, is)
#define AQM1602_SET_DADR(ac6, ac5, ac4, ac3, ac2, ac1, ac0) \
	AQM1602_IST(1, ac6, ac5, ac4, ac3, ac2, ac1, ac0)

// Table 0 Instructions
#define AQM1602_SET_CRSR(sc, rl) \
	AQM1602_IST(0, 0, 0, 1, sc, rl, 0, 0)
#define AQM1602_SET_CADR(ac5, ac4, ac3, ac2, ac1, ac0) \
	AQM1602_IST(0, 1, ac5, ac4, ac3, ac2, ac1, ac0)

// Table 1 Instructions
#define AQM1602_SET_FREQ(bs, f2, f1, f0) \
	AQM1602_IST(0, 0, 0, 1, bs, f2, f1, f0)
#define AQM1602_SET_ICON(ac3, ac2, ac1, ac0) \
	AQM1602_IST(0, 1, 0, 0, ac3, ac2, ac1, ac0)
#define AQM1602_SET_POWR(ion, bon, c5, c4) \
	AQM1602_IST(0, 1, 0, 1, ion, bon, c5, c4)
#define AQM1602_SET_FLWR(fon, rab2, rab1, rab0) \
	AQM1602_IST(0, 1, 1, 0, fon, rab2, rab1, rab0)
#define AQM1602_SET_CONT(c3, c2, c1, c0) \
	AQM1602_IST(0, 1, 1, 1, c3, c2, c1, c0)

static const int max_col = 16;
static const int max_row = 2;

static int cursor = 0;
static int line_number = 0;

static void writeInst(int fd, uint8_t data)
{
	wiringPiI2CWriteReg8(fd, AQM1602_RS0, data);
	delay(1);
}

static void writeData(int fd, uint8_t data)
{
	wiringPiI2CWriteReg8(fd, AQM1602_RS1, data);
	delay(1);
}

void initLcd(int fd)
{
	uint8_t cmds[] = {
		AQM1602_SET_FUNC(1, 1, 0, 1),
		AQM1602_SET_FREQ(0, 1, 0, 0),
		AQM1602_SET_CONT(0, 0, 1, 1),
		AQM1602_SET_POWR(0, 1, 1, 0),
		AQM1602_SET_FLWR(1, 1, 0, 0),
		AQM1602_SET_FUNC(1, 1, 0, 0),
		AQM1602_CLR_DISP(),
		AQM1602_SET_DISP(1, 0, 0),
	};
	int cmds_len = sizeof(cmds) / sizeof(cmds[0]);
	for (int i = 0; i < cmds_len; i++) {
		writeInst(fd, cmds[i]);
	}
	cursor = 0;
	line_number = 0;
}

void clearLcd(int fd)
{
	writeInst(fd, AQM1602_CLR_DISP());
	cursor = 0;
	line_number = 0;
}

void newlineLcd(int fd)
{
	if (line_number + 1 == max_row) {
		clearLcd(fd);
	}
	else {
		writeInst(fd, AQM1602_SET_DADR(1, 0, 0, 0, 0, 0, 0)); // 0x40
		cursor = 0;
		line_number++;
	}
}

void putcLcd(int fd, char c)
{
	if (cursor == max_col) {
		newlineLcd(fd);
	}
	writeData(fd, c);
	cursor++;
}

void putsLcd(int fd, char *s)
{
	while (*s) {
		putcLcd(fd, *s);
		s++;
	}
}

void setCursorLcd(int fd, int _line_number, int _cursor)
{
	if (_cursor < 0 || max_col < _cursor ||
		_line_number < 0 || max_row <= _line_number)
		return;
	
	cursor = _cursor;
	line_number = _line_number;
	uint8_t address = (_line_number * 0x40 + _cursor) & 0x7F;
	uint8_t inst = AQM1602_SET_DADR(0, 0, 0, 0, 0, 0, 0) | address;
	writeInst(fd, inst);
}

void setCgRamLcd(int fd, uint8_t address, uint8_t *data)
{
	writeInst(fd, AQM1602_SET_CADR(
		(address & 0b100) >> 2,
		(address & 0b010) >> 1,
		(address & 0b001) >> 0,
		0, 0, 0
	));
	for (int i = 0; i < 8; i++) {
		writeData(fd, data[i]);
	}
}

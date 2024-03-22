#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

void initButton(void)
{
	pinMode(IO_SW_RD, INPUT);
	pinMode(IO_SW_WT, INPUT);
}

int getRedButton(void)
{
	return digitalRead(IO_SW_RD);
}

int getWhiteButton(void)
{
	return digitalRead(IO_SW_WT);
}

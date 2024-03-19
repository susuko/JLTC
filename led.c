#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

void initLed(void)
{
	pinMode(IO_LD, OUTPUT);
}

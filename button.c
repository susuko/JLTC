#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

void initButton(void)
{
	pinMode(SW_RED, INPUT);
	pinMode(SW_WHITE, INPUT);
}

#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

void initBuzzer(void)
{
	pinMode(BZ, PWM_OUTPUT);
	pwmSetClock(54); // 1us
	pwmSetMode(PWM_MODE_MS);
}

void setBuzzer(int hz, int ms)
{
	piLock(BZ_LOCK_KEY);
	
	if (hz > 0) {
		int maxRange = 1000 * 1000 / hz;
		pwmSetRange(maxRange);
		pwmWrite(BZ, maxRange / 2);
	}
	else {
		pwmWrite(BZ, 0);
	}
	delay(ms);
	pwmWrite(BZ, 0);
	
	piUnlock(BZ_LOCK_KEY);
}

void playStartBeep(void)
{
	setBuzzer(1000, 100);
	setBuzzer(2000, 100);
}

void playEndBeep(void)
{
	setBuzzer(2000, 100);
	setBuzzer(1000, 100);
}

#include <pthread.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

static pthread_mutex_t buzzer_mutex;

void initBuzzer(void)
{
	pinMode(IO_BZ, PWM_OUTPUT);
	pwmSetClock(54); // 1us
	pwmSetMode(PWM_MODE_MS);
}

void setBuzzer(int hz, int ms)
{
	pthread_mutex_lock(&buzzer_mutex);
	
	if (hz > 0) {
		int maxRange = 1000 * 1000 / hz;
		pwmSetRange(maxRange);
		pwmWrite(IO_BZ, maxRange / 2);
	}
	else {
		pwmWrite(IO_BZ, 0);
	}
	delay(ms);
	pwmWrite(IO_BZ, 0);
	
	pthread_mutex_unlock(&buzzer_mutex);
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

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <jeedPi.h>
#include <jltc.h>

int main(void)
{
	wiringPiSetupGpio();
	
	initButton();
	initBuzzer();
	initLineSensor();
	
	int lcd_fd = wiringPiI2CSetup(AQM1602_ADR);
	initLcd(lcd_fd);
	
	startShutdownThread(lcd_fd);
	startStraightLineDetectionThread();
	
	// setBuzzer(1000, 100);
	// setBuzzer(2000, 100);
	
	while (1) {
		delay(10);
	}
	
	return 0;
}

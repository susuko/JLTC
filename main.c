#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <jeedPi.h>
#include <jltc.h>

int main(void)
{
	wiringPiSetupGpio();
	
	int lcd_fd = wiringPiI2CSetup(AQM1602_ADR);
	int dist_fd = wiringPiI2CSetup(GP2Y0E03_ADR);
	
	initButton();
	initBuzzer();
	initLineSensor();
	initMotor();
	initServo();
	initLcd(lcd_fd);
	
	startShutdownThread(lcd_fd);
	startStraightLineDetectionThread();
	startNearDistanceWarningThread(dist_fd);
	
	// setBuzzer(1000, 100);
	// setBuzzer(2000, 100);
	
	while (1) {
		delay(10);
	}
	
	return 0;
}

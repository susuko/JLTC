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
	initLed();
	initBuzzer();
	initLineSensor();
	initMotor();
	initServo();
	initLcd(lcd_fd);
	
	startShutdownManagementThread(lcd_fd);
	startStraightLineDetectionThread();
	startDistanceMonitoringThread(dist_fd);
	startLoggerThread();
	startHeadAngleControlThread();
	startLocationManagementThread();
	
	// playStartBeep();
	
	waitStartButton(lcd_fd);
	startControlRobot();
	
	return 0;
}

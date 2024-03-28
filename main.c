#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <jeedPi.h>
#include <jltc.h>

void init(int lcd_fd, int dist_fd)
{
	initButton();
	initLed();
	initBuzzer();
	initLineSensor();
	initMotor();
	initServo();
	initLcd(lcd_fd);
	
	startLoggerThread();
	startShutdownManagementThread(lcd_fd);
	startStraightLineMonitoringThread();
	startDistanceMonitoringThread(dist_fd);
	startHeadAngleControlThread();
	startPositionMonitoringThread();
}

int main(void)
{
	wiringPiSetupGpio();
	
	int lcd_fd = wiringPiI2CSetup(AQM1602_ADR);
	int dist_fd = wiringPiI2CSetup(GP2Y0E03_ADR);
	
	init(lcd_fd, dist_fd);
	
	playStartBeep();
	
	while (1) {
		waitStartButton(lcd_fd);
		startControlRobot(lcd_fd);
	}
	
	return 0;
}

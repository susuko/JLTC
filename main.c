#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <jeedPi.h>
#include <jltc.h>

int main(void)
{
	wiringPiSetupGpio();
	
	pinMode(SW_RED, INPUT);
	
	int lcd_fd = wiringPiI2CSetup(AQM1602_ADR);
	initLcd(lcd_fd);
	
	start_shutdown_thread(lcd_fd);
	
	// TODO: Add Beep
	
	while (1) {
		delay(10);
	}
	
	return 0;
}

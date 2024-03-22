#include <stdlib.h>
#include <stdnoreturn.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

static int _lcd_fd;

static noreturn void shutdown(void)
{
	system("shutdown -h now");
	exit(0);
}

static PI_THREAD (shutdownManagementThread)
{
	int counter = 0;
	while(counter < 3) {
		counter = getRedButton() ? counter + 1 : 0;
		delay(1000);
	}
	
	playEndBeep();
	
	clearLcd(_lcd_fd);
	putsLcd(_lcd_fd, "SHUTDOWN...");
	
	shutdown();
	
	return NULL;
}

void startShutdownManagementThread(int lcd_fd)
{
	_lcd_fd = lcd_fd;
	piThreadCreate(shutdownManagementThread);
}

#include <stdlib.h>
#include <stdnoreturn.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

static int _lcd_fd;

static noreturn void shutdown(void)
{
	logPrintf("shutdown", "1");
	playEndBeep();
	clearLcd(_lcd_fd);
	putsLcd(_lcd_fd, "Shutdown...");
	
	system("shutdown -h now");
	exit(0);
}

static PI_THREAD (shutdownManagementThread)
{
	const int SHUTDOWN_COUNT = 3; // [s]
	
	int counter = 0;
	while(counter < SHUTDOWN_COUNT) {
		counter = getRedButton() ? counter + 1 : 0;
		delay(1000);
	}
	
	shutdown();
	
	return NULL;
}

void startShutdownManagementThread(int lcd_fd)
{
	_lcd_fd = lcd_fd;
	piThreadCreate(shutdownManagementThread);
}

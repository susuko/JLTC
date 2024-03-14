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

static PI_THREAD (shutdown_thread)
{
	int counter = 0;
	while(counter < 3) {
		counter = digitalRead(SW_RED) ? counter + 1 : 0;
		delay(1000);
	}
	
	// TODO: Add Beep
	// TODO: Lock Buzzer
	// TODO: Lock LCD
	
	clearLcd(_lcd_fd);
	putsLcd(_lcd_fd, "SHUTDOWN...");
	
	shutdown();
	
	return NULL;
}

void start_shutdown_thread(int lcd_fd)
{
	_lcd_fd = lcd_fd;
	piThreadCreate(shutdown_thread);
}

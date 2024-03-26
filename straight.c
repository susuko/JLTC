#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

static FILE *ninepx_fp;
static int _in_straight_line = 0;

static int detectInStraightLine(int *px)
{
	const int black_th = 64, white_th = 96;
	
	return px[0] < black_th && px[1] > white_th && px[2] < black_th
		&& px[3] < black_th && px[4] > white_th && px[5] < black_th
		&& px[6] < black_th && px[7] > white_th && px[8] < black_th;
}

static PI_THREAD (straightLineMonitoringThread)
{
	int px[9];
	while (1) {
		for (int i = 0; i < 9; i++) {
			fscanf(ninepx_fp, "%d", &px[i]);
		}
		
		int new_in_straight_line = detectInStraightLine(px);
		if (_in_straight_line != new_in_straight_line) {
			logPrintf("straightLineMonitoringThread", "%d", new_in_straight_line);
		}
		_in_straight_line = new_in_straight_line;
		
		delay(50);
	}
	return NULL;
}

int getInStraightLine(void)
{
	return _in_straight_line;
}

void startStraightLineMonitoringThread(void)
{
	ninepx_fp = popen(PYTHON_PATH " " NINEPX_PATH, "r");
	piThreadCreate(straightLineMonitoringThread);
}

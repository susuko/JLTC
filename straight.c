#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

static FILE *ninepx_fp;
static int _in_straight_line = 0;

static int detectInStraightLine(int *px)
{
	int max = INT_MIN, min = INT_MAX;
	
	for (int i = 0; i < 9; i++) {
		if (max < px[i]) {
			max = px[i];
		}
		if (min > px[i]) {
			min = px[i];
		}
	}
	
	int mid = (max + min) / 2;
	
	return px[0] < mid && px[1] > mid && px[2] < mid
		&& px[3] < mid && px[4] > mid && px[5] < mid
		&& px[6] < mid && px[7] > mid && px[8] < mid;
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
		
		setLed(_in_straight_line);
		
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

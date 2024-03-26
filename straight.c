#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

static FILE *ninepx_fp;
static int straight_line_detection = 0;

static int detectStraightLine(int *px)
{
	const int black_th = 64, white_th = 96;
	
	return px[0] < black_th && px[1] > white_th && px[2] < black_th
		&& px[3] < black_th && px[4] > white_th && px[5] < black_th
		&& px[6] < black_th && px[7] > white_th && px[8] < black_th;
}

static PI_THREAD (straightLineDetectionThread)
{
	int px[9];
	while (1) {
		for (int i = 0; i < 9; i++) {
			fscanf(ninepx_fp, "%d", &px[i]);
		}
		
		int now_straight_line_detection = detectStraightLine(px);
		if (straight_line_detection != now_straight_line_detection) {
			logPrintf("straightLineDetectionThread", "%d", now_straight_line_detection);
		}
		straight_line_detection = now_straight_line_detection;
		
		delay(50);
	}
	return NULL;
}

int getStraightLineDetection(void)
{
	return straight_line_detection;
}

void startStraightLineDetectionThread(void)
{
	ninepx_fp = popen(PYTHON_PATH " " NINEPX_PATH, "r");
	piThreadCreate(straightLineDetectionThread);
}

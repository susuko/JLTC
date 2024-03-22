#include <stdlib.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

// Warning threshold [cm]
static const int dist_th = 15;

static int _dist_fd;
static int nearDistanceWarning = 0;

static PI_THREAD (nearDistanceWarningThread)
{
	int prev_dist = 0;
	
	while (1) {
		int dist = readDist(_dist_fd);
		nearDistanceWarning = dist < dist_th;
		if (prev_dist != dist) {
			logPrintf("nearDistanceWarningThread", "%d", dist);
			prev_dist = dist;
		}
		delay(50);
	}
	
	return NULL;
}

int getNearDistanceWarning(void)
{
	return nearDistanceWarning;
}

void startNearDistanceWarningThread(int dist_fd)
{
	_dist_fd = dist_fd;
	piThreadCreate(nearDistanceWarningThread);
}

#include <stdlib.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

// Warning threshold [cm]
static const double DIST_TH = 15;

static int _dist_fd;
static int _distance_warning = 0;

static PI_THREAD (distanceMonitoringThread)
{
	double prev_dist = 0;
	
	while (1) {
		double dist = readDist(_dist_fd);
		_distance_warning = prev_dist < DIST_TH && dist < DIST_TH;
		
		if (prev_dist != dist) {
			logPrintf("distanceMonitoringThread", "%.2f", dist);
		}
		
		prev_dist = dist;
		delay(50);
	}
	
	return NULL;
}

int getDistanceWarning(void)
{
	return _distance_warning;
}

void startDistanceMonitoringThread(int dist_fd)
{
	_dist_fd = dist_fd;
	piThreadCreate(distanceMonitoringThread);
}

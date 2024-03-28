#include <stdlib.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

// Warning threshold [cm]
static const double DIST_TH = 15;
// Update cycle [ms]
static const int UPDATE_CYCLE = 50;
// Warning duration = UPDATE_CYCLE * WARNING_CYCLE_COUNT [ms]
static const int WARNING_CYCLE_COUNT = 4;

static int _dist_fd;
static int _distance_warning = 0;

static void updateDistanceWarning(double prev_dist, double now_dist)
{
	static int warning_count = 0;
	
	if (prev_dist < DIST_TH && now_dist < DIST_TH) {
		warning_count = WARNING_CYCLE_COUNT;
	}
	
	if (warning_count > 0) {
		_distance_warning = 1;
		warning_count--;
	}
	else {
		_distance_warning = 0;
	}
}

static PI_THREAD (distanceMonitoringThread)
{
	double prev_dist = 0;
	
	while (1) {
		double now_dist = readDist(_dist_fd);
		
		updateDistanceWarning(prev_dist, now_dist);
		
		if (prev_dist != now_dist) {
			logPrintf("distanceMonitoringThread", "%.2f", now_dist);
		}
		
		prev_dist = now_dist;
		delay(UPDATE_CYCLE);
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

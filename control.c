#include <stdint.h>
#include <math.h>
#include <wiringPi.h>
#include <jltc.h>

void startControlRobot(void)
{
	const uint32_t time_limit = 500;
	
	int last_dir = 0;
	uint32_t last_dir_time = millis() - time_limit;
	uint32_t last_straight_time = millis() - time_limit;
	
	while (1) {
		uint32_t now = millis();
		double line_dist = getLineDist();
		
		if (getNearDistanceWarning()) {
			setMotorXy(0.0, 0.0);
		}
		else if (isfinite(line_dist)) {
			if (getStraightLineDetection()) {
				last_straight_time = now;
			}
			if (line_dist != 0.0) {
				last_dir = line_dist < 0 ? -1 : 1;
				last_dir_time = now;
			}
			
			if (now < last_straight_time + time_limit) {
				setMotorXy(line_dist * 0.25, 1.0);
			}
			else {
				setMotorXy(line_dist * 0.75, 1.0);
			}
		}
		else {
			if (now < last_dir_time + time_limit) {
				setMotorXy(last_dir * 0.75, 1.0);
			}
			else {
				setMotorXy(0.0, 0.0);
			}
		}
		delay(10);
	}
}

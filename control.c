#include <stdint.h>
#include <math.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

void waitStartButton(int lcd_fd)
{
	clearLcd(lcd_fd);
	putsLcd(lcd_fd, "Push WhiteButton To Start");
	
	while (1) {
		if (getWhiteButton())
			break;
	}
	
	clearLcd(lcd_fd);
}

void startControlRobot(void)
{
	const uint32_t time_limit = 500;
	
	int last_dir = 0;
	uint32_t start_time = millis();
	int64_t last_dir_time = INT32_MIN;
	int64_t last_straight_time = INT32_MIN;
	
	while (1) {
		uint32_t now = millis() - start_time;
		double line_dist = getLineDist();
		
		if (getDistanceWarning()) {
			setMotorXy(0.0, 0.0);
		}
		else if (isfinite(line_dist)) {
			if (getInStraightLine()) {
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
			else if (now < last_dir_time + time_limit * 2) {
				setMotorXy(last_dir * 0.75, -1.0);
			}
			else if (now < last_dir_time + time_limit * 3) {
				setMotorXy(last_dir * -0.75, 1.0);
			}
			else {
				setMotorXy(0.0, 0.0);
			}
		}
		
		delay(10);
	}
}

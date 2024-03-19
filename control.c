#include <math.h>
#include <wiringPi.h>
#include <jltc.h>

void startControlRobot(void)
{
	while (1) {
		double line_dist = getLineDist();
		if (isfinite(line_dist)) {
			setMotorXy(line_dist, 1.0);
		}
		else {
			setMotor(0.0, 0.0);
		}
		delay(10);
	}
}

#include <wiringPi.h>
#include <jltc.h>

void startControlRobot(void)
{
	while (1) {
		t_line_sensor line_sensor = getLineSensor();
		if (line_sensor.left && !line_sensor.right) {
			setMotor(0.0, 1.0);
		}
		else if (!line_sensor.left && line_sensor.right) {
			setMotor(1.0, 0.0);
		}
		else if (line_sensor.centor) {
			setMotor(1.0, 1.0);
		}
		else {
			setMotor(0.0, 0.0);
		}
		delay(10);
	}
}

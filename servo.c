#include <math.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <jeedPi.h>
#include <jltc.h>

// Cycle is 20ms (50Hz)
static const int pwm_range = 200;
// SG-90 limit
static const int sg90_min_level = 5;
static const int sg90_max_level = 24;
// Hardware limit
static const int min_level = 5;
static const int max_level = 16;
// Mounting angle when pulse is 0.5ms (Degree)
static const int mounting_degree = 35;

void initServo(void)
{
	pinMode(SERVO, OUTPUT);
	softPwmCreate(SERVO, 0, pwm_range);
}

static void setServoRaw(int level)
{
	if (level < min_level) {
		level = min_level;
	}
	if (level > max_level) {
		level = max_level;
	}
	softPwmWrite(SERVO, level);
}

static void stopServo(void)
{
	softPwmWrite(SERVO, 0);
}

// angle: -PI/2 ... PI/2 [rad]
void setServo(double angle)
{
	if (angle < -M_PI_2 || angle > M_PI_2) {
		return;
	}
	double angle_unit = M_PI / (sg90_max_level - sg90_min_level);
	double mounting_angle = (M_PI / 180) * mounting_degree;
	double offset_angle = M_PI_2 - mounting_angle;
	int level = sg90_min_level + round((angle + offset_angle) / angle_unit);
	
	setServoRaw(level);
	
	// Wait 3 Cycle
	int cycle_ms = pwm_range / 10;
	delay(cycle_ms * 3);
	
	stopServo();
}

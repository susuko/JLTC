#include <stddef.h>
#include <math.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <jeedPi.h>
#include <jltc.h>

// Cycle is 20ms (50Hz)
static const int PWM_RANGE = 200;
// SG-90 limit
static const int SG90_MIN_LEVEL = 5;
static const int SG90_MAX_LEVEL = 24;
// Hardware limit
static const int MIN_LEVEL = 5;
static const int MAX_LEVEL = 16;
// Mounting angle when pulse is 0.5ms (Degree)
static const int MOUNTING_DEGREE = 35;

static void setServoRaw(int level)
{
	if (level < MIN_LEVEL) {
		level = MIN_LEVEL;
	}
	if (level > MAX_LEVEL) {
		level = MAX_LEVEL;
	}
	softPwmWrite(IO_SV, level);
}

static void stopServo(void)
{
	softPwmWrite(IO_SV, 0);
}

static void logSetServo(double now_angle)
{
	static double last_angle = 0;
	
	if (last_angle != now_angle) {
		logPrintf("setServo", "%f", now_angle);
	}
	
	last_angle = now_angle;
}

// angle: -PI/2 ... PI/2 [rad]
void setServo(double angle)
{
	if (angle < -M_PI_2 || angle > M_PI_2) {
		return;
	}
	
	logSetServo(angle);
	
	double angle_unit = M_PI / (SG90_MAX_LEVEL - SG90_MIN_LEVEL);
	double mounting_angle = (M_PI / 180) * MOUNTING_DEGREE;
	double offset_angle = M_PI_2 - mounting_angle;
	int level = SG90_MIN_LEVEL + round((angle + offset_angle) / angle_unit);
	
	setServoRaw(level);
	
	// Wait 3 Cycle
	int cycle_ms = PWM_RANGE / 10;
	delay(cycle_ms * 3);
	
	stopServo();
}

void initServo(void)
{
	pinMode(IO_SV, OUTPUT);
	softPwmCreate(IO_SV, 0, PWM_RANGE);
}

static PI_THREAD (headAngleControlThread)
{
	const double LR_DIFF_TH = 1.0;
	const double TARGET_ANGLE = M_PI / 6.0;
	
	double prev_angle = 0;
	
	while (1) {
		t_vec2 lr = getMotor();
		double lr_diff = lr.y - lr.x;
		double lr_diff_sign = lr_diff < 0 ? -1 : 1;
		
		double angle = fabs(lr_diff) >= LR_DIFF_TH
			? TARGET_ANGLE * lr_diff_sign
			: 0;
		
		if (prev_angle != angle)
			setServo(angle);
		prev_angle = angle;
		
		delay(1000);
	}
	
	return NULL;
}

void startHeadAngleControlThread(void)
{
	piThreadCreate(headAngleControlThread);
}

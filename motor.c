#include <math.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <jeedPi.h>
#include <jltc.h>

static const int pwm_range = 4;

void initMotor(void)
{
	pinMode(MT_R2, OUTPUT);
	pinMode(MT_R1, OUTPUT);
	pinMode(MT_L2, OUTPUT);
	pinMode(MT_L1, OUTPUT);
	
	softPwmCreate(MT_R2, 0, pwm_range);
	softPwmCreate(MT_R1, 0, pwm_range);
	softPwmCreate(MT_L2, 0, pwm_range);
	softPwmCreate(MT_L1, 0, pwm_range);
}

// left, right: -1.0 ... 1.0
void setMotor(double left, double right)
{
	int left_value = (int)round(fabs(left) * pwm_range);
	int right_value = (int)round(fabs(right) * pwm_range);
	
	softPwmWrite(MT_L1, left > 0 ? left_value : 0);
	softPwmWrite(MT_L2, left < 0 ? left_value : 0);
	softPwmWrite(MT_R1, right > 0 ? right_value : 0);
	softPwmWrite(MT_R2, right < 0 ? right_value : 0);
}

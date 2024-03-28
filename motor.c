#include <math.h>
#include <assert.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <jeedPi.h>
#include <jltc.h>

static const int PWM_RANGE = 4;

static pthread_mutex_t motor_mutex;

static t_vec2 _last_lr = { 0, 0 };

static t_vec2 xyToLr(t_vec2 xy)
{
	double x_is_neg = xy.x < 0, x_sign = x_is_neg ? -1 : 1;
	double v = xy.y - 2 * xy.x * xy.y * x_sign;
	return (x_is_neg)
		? (t_vec2){ v, xy.y }
		: (t_vec2){ xy.y, v };
}

// Test for xyToLr
static int testXyToLr(void)
{
	int result = 1;
	result = result && vec2Eq(xyToLr((t_vec2){ -1.0, -1.0 }), (t_vec2){  1.0, -1.0 });
	result = result && vec2Eq(xyToLr((t_vec2){ -1.0, -0.5 }), (t_vec2){  0.5, -0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){ -1.0,  0.0 }), (t_vec2){  0.0,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){ -1.0,  0.5 }), (t_vec2){ -0.5,  0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){ -1.0,  1.0 }), (t_vec2){ -1.0,  1.0 });
	result = result && vec2Eq(xyToLr((t_vec2){ -0.5, -1.0 }), (t_vec2){  0.0, -1.0 });
	result = result && vec2Eq(xyToLr((t_vec2){ -0.5, -0.5 }), (t_vec2){  0.0, -0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){ -0.5,  0.0 }), (t_vec2){  0.0,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){ -0.5,  0.5 }), (t_vec2){  0.0,  0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){ -0.5,  1.0 }), (t_vec2){  0.0,  1.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.0, -1.0 }), (t_vec2){ -1.0, -1.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.0, -0.5 }), (t_vec2){ -0.5, -0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.0,  0.0 }), (t_vec2){  0.0,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.0,  0.5 }), (t_vec2){  0.5,  0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.0,  1.0 }), (t_vec2){  1.0,  1.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.5, -1.0 }), (t_vec2){ -1.0,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.5, -0.5 }), (t_vec2){ -0.5,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.5,  0.0 }), (t_vec2){  0.0,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.5,  0.5 }), (t_vec2){  0.5,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  0.5,  1.0 }), (t_vec2){  1.0,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  1.0, -1.0 }), (t_vec2){ -1.0,  1.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  1.0, -0.5 }), (t_vec2){ -0.5,  0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){  1.0,  0.0 }), (t_vec2){  0.0,  0.0 });
	result = result && vec2Eq(xyToLr((t_vec2){  1.0,  0.5 }), (t_vec2){  0.5, -0.5 });
	result = result && vec2Eq(xyToLr((t_vec2){  1.0,  1.0 }), (t_vec2){  1.0, -1.0 });
	return result;
}

// left(lr.x), right(lr.y): -1.0 ... 1.0
void setMotor(t_vec2 lr)
{
	int left_value = (int)round(fabs(lr.x) * PWM_RANGE);
	int right_value = (int)round(fabs(lr.y) * PWM_RANGE);
	
	if (!vec2Eq(_last_lr, lr)) {
		logPrintf("setMotor", "%.2f, %.2f", lr.x, lr.y);
	}
	
	softPwmWrite(IO_MT_L1, lr.x > 0 ? left_value : 0);
	softPwmWrite(IO_MT_L2, lr.x < 0 ? left_value : 0);
	softPwmWrite(IO_MT_R1, lr.y > 0 ? right_value : 0);
	softPwmWrite(IO_MT_R2, lr.y < 0 ? right_value : 0);
	
	pthread_mutex_lock(&motor_mutex);
	
	_last_lr = lr;
	
	pthread_mutex_unlock(&motor_mutex);
}

// xy.x, xy.y: -1.0 ... 1.0
void setMotorXy(t_vec2 xy)
{
	setMotor(xyToLr(xy));
}

t_vec2 getMotor(void)
{
	pthread_mutex_lock(&motor_mutex);
	
	t_vec2 last_lr = _last_lr;
	
	pthread_mutex_unlock(&motor_mutex);
	
	return last_lr;
}

void initMotor(void)
{
	assert(testXyToLr());
	
	pinMode(IO_MT_R2, OUTPUT);
	pinMode(IO_MT_R1, OUTPUT);
	pinMode(IO_MT_L2, OUTPUT);
	pinMode(IO_MT_L1, OUTPUT);
	
	softPwmCreate(IO_MT_R2, 0, PWM_RANGE);
	softPwmCreate(IO_MT_R1, 0, PWM_RANGE);
	softPwmCreate(IO_MT_L2, 0, PWM_RANGE);
	softPwmCreate(IO_MT_L1, 0, PWM_RANGE);
}

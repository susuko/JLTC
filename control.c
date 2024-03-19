#include <math.h>
#include <assert.h>
#include <wiringPi.h>
#include <jltc.h>

typedef struct s_vec2 {
	double x;
	double y;
} t_vec2;

int vec2Eq(t_vec2 a, t_vec2 b)
{
	return a.x == b.x && a.y == b.y;
}

// return: -1.0 ... 1.0 or INFINITY or NAN
double calcLineDist(t_line_sensor line_sensor)
{
	int count = line_sensor.left + line_sensor.centor + line_sensor.right;
	double sum = line_sensor.left * -1.0 + line_sensor.right * 1.0;
	double avg = sum / count;
	
	if (line_sensor.left && line_sensor.right) {
		return NAN;
	}
	else if (!line_sensor.left && !line_sensor.centor && !line_sensor.right) {
		return INFINITY;
	}
	else {
		return avg;
	}
}

// Test calcLineDist
int testCalcLineDist(void)
{
	int result = 1;
	result = result && isinf ( calcLineDist((t_line_sensor){ 0, 0, 0 }));
	result = result && +1.0 == calcLineDist((t_line_sensor){ 0, 0, 1 });
	result = result && +0.0 == calcLineDist((t_line_sensor){ 0, 1, 0 });
	result = result && +0.5 == calcLineDist((t_line_sensor){ 0, 1, 1 });
	result = result && -1.0 == calcLineDist((t_line_sensor){ 1, 0, 0 });
	result = result && isnan ( calcLineDist((t_line_sensor){ 1, 0, 1 }));
	result = result && -0.5 == calcLineDist((t_line_sensor){ 1, 1, 0 });
	result = result && isnan ( calcLineDist((t_line_sensor){ 1, 1, 1 }));
	return result;
}

t_vec2 xyToLr(t_vec2 xy)
{
	double x_is_neg = xy.x < 0, x_sign = x_is_neg ? -1 : 1;
	double v = xy.y - 2 * xy.x * xy.y * x_sign;
	return (x_is_neg)
		? (t_vec2){ v, xy.y }
		: (t_vec2){ xy.y, v };
}

// Test xyToLr
int testXyToLr(void)
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

void startControlRobot(void)
{
	assert(testCalcLineDist());
	assert(testXyToLr());
	
	while (1) {
		t_line_sensor line_sensor = getLineSensor();
		double line_dist = calcLineDist(line_sensor);
		if (isfinite(line_dist)) {
			t_vec2 xy = { line_dist, 1.0 };
			t_vec2 lr = xyToLr(xy);
			setMotor(lr.x, lr.y);
		}
		else {
			setMotor(0.0, 0.0);
		}
		delay(10);
	}
}

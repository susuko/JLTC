#include <math.h>
#include <assert.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

// return: -1.0 ... 1.0 or INFINITY or NAN
static double calcLineDist(t_line_sensor line_sensor)
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

// Test for calcLineDist
static int testCalcLineDist(void)
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

// White: 1, Black: 0
t_line_sensor getLineSensor(void)
{
	static t_line_sensor last = { 0, 0, 0 };
	
	t_line_sensor now = {
		.left = digitalRead(IO_LN_LE),
		.centor = !digitalRead(IO_LN_CT), // invert
		.right = digitalRead(IO_LN_RI)
	};
	
	if (last.left != now.left || last.centor != now.centor || last.right != now.right) {
		logPrintf("getLineSensor", "%d, %d, %d", now.left, now.centor, now.right);
	}
	last = now;
	
	return now;
}

double getLineDist(void)
{
	return calcLineDist(getLineSensor());
}

void initLineSensor(void)
{
	assert(testCalcLineDist());
	
	pinMode(IO_LN_LE, INPUT);
	pinMode(IO_LN_CT, INPUT);
	pinMode(IO_LN_RI, INPUT);
}

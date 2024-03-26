#include <math.h>
#include <assert.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

// return: -1.0 ... 1.0 or INFINITY or NAN
static double calcLineDist(t_vec3 line_sensor)
{
	int count = line_sensor.x + line_sensor.y + line_sensor.z;
	double sum = line_sensor.x * -1.0 + line_sensor.z * 1.0;
	double avg = sum / count;
	
	if (line_sensor.x && line_sensor.z) {
		return NAN;
	}
	else if (!line_sensor.x && !line_sensor.y && !line_sensor.z) {
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
	result = result && isinf ( calcLineDist((t_vec3){ 0, 0, 0 }));
	result = result && +1.0 == calcLineDist((t_vec3){ 0, 0, 1 });
	result = result && +0.0 == calcLineDist((t_vec3){ 0, 1, 0 });
	result = result && +0.5 == calcLineDist((t_vec3){ 0, 1, 1 });
	result = result && -1.0 == calcLineDist((t_vec3){ 1, 0, 0 });
	result = result && isnan ( calcLineDist((t_vec3){ 1, 0, 1 }));
	result = result && -0.5 == calcLineDist((t_vec3){ 1, 1, 0 });
	result = result && isnan ( calcLineDist((t_vec3){ 1, 1, 1 }));
	return result;
}

// White: 1, Black: 0
// Left: .x, Center: .y, Right: .z
t_vec3 getLineSensor(void)
{
	static t_vec3 last = { 0, 0, 0 };
	
	t_vec3 now = {
		.x = digitalRead(IO_LN_LE),
		.y = !digitalRead(IO_LN_CT), // invert
		.z = digitalRead(IO_LN_RI)
	};
	
	if (!vec3Eq(last, now)) {
		logPrintf("getLineSensor", "%.0f, %.0f, %.0f", now.x, now.y, now.z);
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

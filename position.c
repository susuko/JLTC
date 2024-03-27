#include <stddef.h>
#include <math.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

static t_vec3 calcPos(t_vec3 pos, double delta_t, t_vec2 motor_lr, double max_v, double wheel_d)
{
	double v_l = max_v * motor_lr.x;
	double v_r = max_v * motor_lr.y;
	double v = (v_r + v_l) / 2.0;
	double w = (v_r - v_l) / wheel_d;
	double delta_d = v * delta_t;
	double delta_a = w * delta_t;
	return (t_vec3) {
		.x = pos.x + delta_d * cos(pos.z + delta_a / 2.0),
		.y = pos.y + delta_d * sin(pos.z + delta_a / 2.0),
		.z = pos.z + delta_a
	};
}

static PI_THREAD (positionMonitoringThread)
{
	const double max_speed = 0.3; // [m/s]
	const double wheel_dist = 0.1; // [m]
	
	uint32_t prev_ms = millis();
	t_vec3 pos = { 0, 0, 0 };
	
	unsigned log_limit_count = 0;
	t_vec3 last_log_pos = pos;
	
	while (1) {
		uint32_t now_ms = millis();
		double delta_time = (now_ms - prev_ms) / 1000.0;
		pos = calcPos(pos, delta_time, getMotor(), max_speed, wheel_dist);
		
		if (log_limit_count >= 10 && !vec3Eq(last_log_pos, pos)) {
			logPrintf("positionMonitoringThread", "%f, %f, %f", pos.x, pos.y, pos.z);
			log_limit_count = 0;
			last_log_pos = pos;
		}
		else {
			log_limit_count++;
		}
		
		prev_ms = now_ms;
		delay(10);
	}
	return NULL;
}

void startPositionMonitoringThread(void)
{
	piThreadCreate(positionMonitoringThread);
}

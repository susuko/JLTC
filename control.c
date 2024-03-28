#include <stdint.h>
#include <math.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

// Duration constant [ms]
static const uint32_t IN_STRAIGHT_LINE_DURATION = 500;
static const uint32_t TURN_MAIN_DURATION = 750;
static const uint32_t TURN_RECOVERY_DURATION = 750;
static const uint32_t TURN_RETRY_DURATION = 750;

typedef enum e_turn_state {
	TURN_MAIN,
	TURN_RECOVERY,
	TURN_RETRY,
	TURN_STOP,
} t_turn_state;

static t_turn_state getTurnState(uint32_t now_ms, uint32_t last_in_line_time)
{
	uint32_t time_sum = last_in_line_time;
	
	if ((time_sum += TURN_MAIN_DURATION) >= now_ms) {
		return TURN_MAIN;
	}
	if ((time_sum += TURN_RECOVERY_DURATION) >= now_ms) {
		return TURN_RECOVERY;
	}
	if ((time_sum += TURN_RETRY_DURATION) >= now_ms) {
		return TURN_RETRY;
	}
	return TURN_STOP;
}

static t_vec2 calcMotorXyInLine(uint32_t now_ms, double line_dist)
{
	static int64_t last_in_straight_line_time = -1;
	
	if (getInStraightLine()) {
		last_in_straight_line_time = now_ms;
	}
	
	int is_in_straight_line = last_in_straight_line_time >= 0 &&
		last_in_straight_line_time + IN_STRAIGHT_LINE_DURATION >= now_ms;

	if (is_in_straight_line) {
		return (t_vec2) { line_dist * 0.25, 1.0 };
	}
	else {
		return (t_vec2) { line_dist * 0.50, 1.0 };
	}
}

static t_vec2 calcMotorXyOutLine(uint32_t now_ms, int last_dir, uint32_t last_in_line_time)
{
	switch (getTurnState(now_ms, last_in_line_time)) {
	case TURN_MAIN:
		return (t_vec2) { last_dir * 0.75, 1.0 };
	
	case TURN_RECOVERY:
		return (t_vec2) { last_dir * 0.75, -1.0 };
	
	case TURN_RETRY:
		return (t_vec2) { last_dir * -0.75, 1.0 };
	
	case TURN_STOP:
	default:
		return (t_vec2) { 0.0, 0.0 };
	}
}

static t_vec2 calcMotorXy(uint32_t now_ms, double line_dist)
{
	static int last_dir = 0;
	static int64_t last_in_line_time = -1;
	
	if (getDistanceWarning()) {
		return (t_vec2) { 0.0, 0.0 };
	}
	
	if (isfinite(line_dist)) {
		if (line_dist != 0) {
			last_dir = line_dist < 0 ? -1 : 1;
		}
		last_in_line_time = now_ms;
		return calcMotorXyInLine(now_ms, line_dist);
	}
	else if (last_in_line_time >= 0 && last_dir != 0) {
		return calcMotorXyOutLine(now_ms, last_dir, last_in_line_time);
	}
	
	return (t_vec2) { 0.0, 0.0 };
}

void startControlRobot(int lcd_fd)
{
	clearLcd(lcd_fd);
	putsLcd(lcd_fd,
		"Push RedButton  "
		" To Stop        "
	);
	
	const int STOP_COUNT = 3;
	
	int counter = 0;
	while (1) {
		counter = getRedButton() ? counter + 1 : 0;
		if (counter >= STOP_COUNT) {
			break;
		}
		
		uint32_t now_ms = millis();
		double line_dist = getLineDist();
		t_vec2 motor_xy = calcMotorXy(now_ms, line_dist);
		
		setMotorXy(motor_xy);
		
		delay(10);
	}
	
	setMotor((t_vec2) { 0.0, 0.0 });
	clearLcd(lcd_fd);
}

void waitStartButton(int lcd_fd)
{
	clearLcd(lcd_fd);
	putsLcd(lcd_fd,
		"Push WhiteButton"
		" To Start       "
	);
	
	while (1) {
		if (getWhiteButton())
			break;
	}
	
	clearLcd(lcd_fd);
}

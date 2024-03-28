#include <stdint.h>
#include <math.h>
#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

// Duration constant [ms]
static const uint32_t WARNING_RESPONSE_BACK_DURATION = 200;
static const uint32_t IN_STRAIGHT_LINE_DURATION = 500;
static const uint32_t TURN_MAIN_DURATION = 750;
static const uint32_t TURN_RECOVERY_DURATION = 750;
static const uint32_t TURN_RETRY_DURATION = 750;

typedef enum e_warning_response_state {
	WARNING_RESPONSE_BACK,
	WARNING_RESPONSE_STOP
} t_warning_response_state;

typedef enum e_turn_state {
	TURN_MAIN,
	TURN_RECOVERY,
	TURN_RETRY,
	TURN_STOP,
} t_turn_state;

static t_warning_response_state getWarningResponseState(uint32_t now_ms, uint32_t last_warning_start_time)
{
	if (last_warning_start_time + WARNING_RESPONSE_BACK_DURATION >= now_ms) {
		return WARNING_RESPONSE_BACK;
	}
	else {
		return WARNING_RESPONSE_STOP;
	}
}

// During distance warning,
// 1. Back. (WARNING_RESPONSE_BACK_DURATION[ms])
// 2. Stop.
static t_vec2 calcMotorXyInWarning(uint32_t now_ms, uint32_t last_warning_start_time)
{
	switch (getWarningResponseState(now_ms, last_warning_start_time)) {
	case WARNING_RESPONSE_BACK:
		return (t_vec2) { .x = 0.0, .y = -1.0 };
	
	case WARNING_RESPONSE_STOP:
	default:
		return (t_vec2) { .x = 0.0, .y = 0.0 };
	}
}

static t_turn_state getTurnState(uint32_t now_ms, uint32_t last_in_line_time)
{
	uint32_t time_sum = last_in_line_time;
	
	if ((time_sum += TURN_MAIN_DURATION) >= now_ms) {
		return TURN_MAIN;
	}
	else if ((time_sum += TURN_RECOVERY_DURATION) >= now_ms) {
		return TURN_RECOVERY;
	}
	else if ((time_sum += TURN_RETRY_DURATION) >= now_ms) {
		return TURN_RETRY;
	}
	return TURN_STOP;
}

// Outside the line,
// 1. Turn to last_dir.
// 2. If the line is not found, cancel step 1.
// 3. Turn in the opposite direction to last_dir.
// 4. If the line is not found, stop.
static t_vec2 calcMotorXyOutLine(uint32_t now_ms, int last_dir, uint32_t last_in_line_time)
{
	switch (getTurnState(now_ms, last_in_line_time)) {
	case TURN_MAIN:
		return (t_vec2) { .x = last_dir * 0.75, .y = 1.0 };
	
	case TURN_RECOVERY:
		return (t_vec2) { .x = last_dir * 0.75, .y = -1.0 };
	
	case TURN_RETRY:
		return (t_vec2) { .x = last_dir * -0.75, .y = 1.0 };
	
	case TURN_STOP:
	default:
		return (t_vec2) { .x = 0.0, .y = 0.0 };
	}
}

// Inside the line,
// - Basically, move forward.
// - Turn proportional to line_dist.
// - Reduce turns in a straight line.
static t_vec2 calcMotorXyInLine(uint32_t now_ms, double line_dist)
{
	static int64_t last_in_straight_line_time = -1;
	
	if (getInStraightLine()) {
		last_in_straight_line_time = now_ms;
	}
	
	int is_in_straight_line = last_in_straight_line_time >= 0 &&
		last_in_straight_line_time + IN_STRAIGHT_LINE_DURATION >= now_ms;

	if (is_in_straight_line) {
		return (t_vec2) { .x = line_dist * 0.25, .y = 1.0 };
	}
	else {
		return (t_vec2) { .x = line_dist * 0.50, .y = 1.0 };
	}
}

static t_vec2 calcMotorXy()
{
	static int64_t last_warning_start_time = -1;
	static int64_t last_in_line_time = -1;
	static int last_distance_warning = 0;
	static int last_dir = 0;
	
	uint32_t now_ms = millis();
	double line_dist = getLineDist();
	
	int distance_warning = getDistanceWarning();
	if (!last_distance_warning && distance_warning) {
		last_warning_start_time = now_ms;
	}
	last_distance_warning = distance_warning;
	
	if (last_warning_start_time >= 0 && distance_warning) {
		last_dir = 0;
		last_in_line_time = -1;
		return calcMotorXyInWarning(now_ms, last_warning_start_time);
	}
	else if (isfinite(line_dist)) {
		if (line_dist != 0) {
			last_dir = line_dist < 0 ? -1 : 1;
		}
		last_in_line_time = now_ms;
		return calcMotorXyInLine(now_ms, line_dist);
	}
	else if (last_in_line_time >= 0 && last_dir != 0) {
		return calcMotorXyOutLine(now_ms, last_dir, last_in_line_time);
	}
	
	return (t_vec2) { .x = 0.0, .y = 0.0 };
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
		
		t_vec2 motor_xy = calcMotorXy();
		
		setMotorXy(motor_xy);
		
		delay(10);
	}
	
	setMotor((t_vec2) { .x = 0.0, .y = 0.0 });
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

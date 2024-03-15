#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

void initLineSensor(void)
{
	pinMode(LINE_L, INPUT);
	pinMode(LINE_C, INPUT);
	pinMode(LINE_R, INPUT);
}

// White: 1, Black: 0
t_line_sensor getLineSensor(void)
{
	return (t_line_sensor) {
		.left = digitalRead(LINE_L),
		.centor = !digitalRead(LINE_C), // invert
		.right = digitalRead(LINE_R)
	};
}

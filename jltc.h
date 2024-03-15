#ifndef _JLTC_H
#define _JLTC_H

// GPIO definition
#define LINE_R 4
#define LINE_C 5
#define LINE_L 6
#define SW_RED 17
#define BZ 18

// shutdown
void startShutdownThread(int lcd_fd);

// buzzer
#define BZ_LOCK_KEY 0

void initBuzzer(void);
void setBuzzer(int hz, int ms);

// straight line detection
void startStraightLineDetectionThread();
int getStraightLineDetection(void);

// line sensor
typedef struct s_line_sensor {
	int left;
	int centor;
	int right;
} t_line_sensor;

void initLineSensor(void);
t_line_sensor getLineSensor(void);

#endif

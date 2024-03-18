#ifndef _JLTC_H
#define _JLTC_H

// GPIO definition
#define LINE_R 4
#define LINE_C 5
#define LINE_L 6
#define SERVO 13
#define SW_RED 17
#define BZ 18
#define LED 20
#define MT_R2 22
#define MT_R1 23
#define MT_L2 24
#define MT_L1 25
#define SW_WHITE 27

// control
void startControlRobot(void);

// shutdown
void startShutdownThread(int lcd_fd);

// buzzer
#define BZ_LOCK_KEY 0

void initBuzzer(void);
void setBuzzer(int hz, int ms);

// straight line detection
void startStraightLineDetectionThread();
int getStraightLineDetection(void);

// logger
void startLoggerThread(void);
int logPrintf(char *fmt, ...);

// line sensor
typedef struct s_line_sensor {
	int left;
	int centor;
	int right;
} t_line_sensor;

void initLineSensor(void);
t_line_sensor getLineSensor(void);

// button
void initButton(void);

// led
void initLed(void);

// motor
void initMotor(void);
void setMotor(double left, double right);

// servo
void initServo(void);
void setServo(double angle);

// near distance warning
void startNearDistanceWarningThread(int dist_fd);
int getNearDistanceWarning(void);

#endif

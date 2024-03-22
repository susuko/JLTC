#ifndef _JLTC_H
#define _JLTC_H

// GPIO definition
#define IO_LN_RI	4
#define IO_LN_CT	5
#define IO_LN_LE	6
#define IO_SV		13
#define IO_SW_RD	17
#define IO_BZ		18
#define IO_LD		20
#define IO_MT_R2	22
#define IO_MT_R1	23
#define IO_MT_L2	24
#define IO_MT_L1	25
#define IO_SW_WT	27

// python
#define PYTHON_PATH "python/venv/bin/python3"
#define NINEPX_PATH "python/9px.py"
#define VIEWER_PATH "python/dashboard.py"

// control
void startControlRobot(void);

// shutdown
void startShutdownManagementThread(int lcd_fd);

// buzzer
void initBuzzer(void);
void setBuzzer(int hz, int ms);
void playStartBeep(void);
void playEndBeep(void);

// straight line detection
void startStraightLineDetectionThread();
int getStraightLineDetection(void);

// logger
void startLoggerThread(void);
int logPrintf(char *cmd, char *fmt, ...);

// line sensor
typedef struct s_line_sensor {
	int left;
	int centor;
	int right;
} t_line_sensor;

void initLineSensor(void);
t_line_sensor getLineSensor(void);
double getLineDist(void);

// button
void initButton(void);
int getRedButton(void);
int getWhiteButton(void);

// led
void initLed(void);
void setLed(int v);

// vector
typedef struct s_vec2 {
	double x;
	double y;
} t_vec2;

int vec2Eq(t_vec2 a, t_vec2 b);

// motor
void initMotor(void);
void setMotor(double left, double right);
void setMotorXy(double x, double y);
t_vec2 getMotor(void);

// servo
void initServo(void);
void setServo(double angle);

// near distance warning
void startNearDistanceWarningThread(int dist_fd);
int getNearDistanceWarning(void);

#endif

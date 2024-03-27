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

// Python path
#define PYTHON_PATH "python/venv/bin/python3"
#define NINEPX_PATH "python/9px.py"
#define VIEWER_PATH "python/dashboard.py"

// Logger setting
#define LOG_ENABLE 1

// Vector
typedef struct s_vec2 {
	double x;
	double y;
} t_vec2;

typedef struct s_vec3 {
	double x;
	double y;
	double z;
} t_vec3;

int vec2Eq(t_vec2 a, t_vec2 b);
int vec3Eq(t_vec3 a, t_vec3 b);

// Button
void initButton(void);
int getRedButton(void);
int getWhiteButton(void);

// Led
void initLed(void);
void setLed(int v);

// Line sensor
void initLineSensor(void);
t_vec3 getLineSensor(void);
double getLineDist(void);

// Motor
void initMotor(void);
void setMotor(t_vec2 lr);
void setMotorXy(t_vec2 xy);
t_vec2 getMotor(void);

// Buzzer
void initBuzzer(void);
void setBuzzer(int hz, int ms);
void playStartBeep(void);
void playEndBeep(void);

// Control
void waitStartButton(int lcd_fd);
void startControlRobot(void);

// Straight line monitoring
void startStraightLineMonitoringThread(void);
int getInStraightLine(void);

// Servo, Haed angle control
void startHeadAngleControlThread(void);
void initServo(void);
void setServo(double angle);

// Distance monitoring
void startDistanceMonitoringThread(int dist_fd);
int getDistanceWarning(void);

// Logger
void startLoggerThread(void);
int logPrintf(char *cmd, char *fmt, ...);

// Position monitoring
void startPositionMonitoringThread(void);

// Shutdown management
void startShutdownManagementThread(int lcd_fd);

#endif

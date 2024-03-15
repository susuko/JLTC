#include <wiringPi.h>
#include <jeedPi.h>
#include <jltc.h>

void initMotor()
{
	pinMode(MT_R2, OUTPUT);
	pinMode(MT_R1, OUTPUT);
	pinMode(MT_L2, OUTPUT);
	pinMode(MT_L1, OUTPUT);
}

void setMotor(int l1, int l2, int r1, int r2)
{
	digitalWrite(MT_L1, l1);
	digitalWrite(MT_L2, l2);
	digitalWrite(MT_R1, r1);
	digitalWrite(MT_R2, r2);
}

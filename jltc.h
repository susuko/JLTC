#ifndef _JLTC_H
#define _JLTC_H

// GPIO definition
#define SW_RED 17
#define BZ 18

// shutdown
void start_shutdown_thread(int lcd_fd);

// buzzer
#define BZ_LOCK_KEY 0

void initBuzzer(void);
void setBuzzer(int hz, int ms);

#endif

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <wiringPi.h>
#include <jltc.h>

static FILE *in_pipe, *out_pipe;

static PI_THREAD (loggerThread)
{
	char buf[512];
	while (fgets(buf, sizeof(buf), out_pipe)) {
		printf("%s", buf);
	}
	
	return NULL;
}

static void initLoggerPipe(void)
{
	int pipe_fds[2];
	pipe(pipe_fds);
	out_pipe = fdopen(pipe_fds[0], "r");
	in_pipe = fdopen(pipe_fds[1], "w");
	setvbuf(in_pipe, NULL, _IOLBF, 0);
}

// string length must be less than PIPE_BUF
int logPrintf(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int len = vfprintf(in_pipe, fmt, ap);
    va_end(ap);
    return len;
}

void startLoggerThread(void)
{
	initLoggerPipe();
	piThreadCreate(loggerThread);
}

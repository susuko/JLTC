#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>
#include <wiringPi.h>
#include <jltc.h>

#define LOG_BUF_SIZE PIPE_BUF

static FILE *in_pipe, *out_pipe;
static FILE *viewer_fp;

static PI_THREAD (loggerThread)
{
	char buf[LOG_BUF_SIZE];
	while (fgets(buf, sizeof(buf), out_pipe)) {
		if (LOG_ENABLE) {
			printf("%s", buf);
			fprintf(viewer_fp, "%s", buf);
			fflush(viewer_fp);
		}
	}
	
	return NULL;
}

static void initLoggerPipe(void)
{
	int pipe_fds[2];
	
	pipe(pipe_fds);
	out_pipe = fdopen(pipe_fds[0], "r");
	in_pipe = fdopen(pipe_fds[1], "w");
	
	// Set in_pipe to line buffering.
	setvbuf(in_pipe, NULL, _IOLBF, 0);
}

// Total string length must be less than PIPE_BUF(PIPE_BUF >= 512)
// Each argument must be separated by a ", "
// Example: logPrintf("command", "%d, %d", 1, 2);
int logPrintf(char *cmd, char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	
	int len = 0;
	len += fprintf(in_pipe, "%010u, ", millis());
	len += fprintf(in_pipe, "%s, ", cmd);
	len += vfprintf(in_pipe, fmt, ap);
	len += fprintf(in_pipe, "\n");
	
	va_end(ap);
	return len < PIPE_BUF ? 0 : -1;
}

void startLoggerThread(void)
{
	initLoggerPipe();
	viewer_fp = popen(PYTHON_PATH " " VIEWER_PATH, "w");
	piThreadCreate(loggerThread);
}

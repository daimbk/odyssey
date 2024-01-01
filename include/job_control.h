#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H

#include <sys/types.h>

extern pid_t foreground;

void setup_signal_handlers();
void handle_sigchld(int signo);
void handle_sigint(int signo);
void handle_sigtstp(int signo);

#endif

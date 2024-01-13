#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H

#include <sys/types.h>
#include <stddef.h>

typedef struct Node
{
    pid_t pid;
    char *command;
    struct Node *next;
} Node;

extern pid_t foreground;
extern volatile int exit_flag;
extern Node *head;

void setup_signal_handlers();
void handle_sigchld(int signo);
void handle_sigint(int signo);
void handle_sigtstp(int signo);
void add_job(pid_t pid, const char *command);
const char *get_command_from_pid(pid_t pid);
int get_next_job_id();
void remove_job(pid_t pid);
void print_jobs();
void cleanup_jobs();

#endif

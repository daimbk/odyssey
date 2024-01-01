#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "job_control.h"
#include "prompt.h"

pid_t foreground = -1;

// set up signal handlers
void setup_signal_handlers()
{
    // Ctrl+C
    signal(SIGINT, handle_sigint);
    // Ctrl+Z
    signal(SIGTSTP, handle_sigtstp);
    // child process terminated
    signal(SIGCHLD, handle_sigchld);
}

// handle SIGCHLD (Child process terminated) signal
void handle_sigchld(int signo)
{
    // reap all terminated child processes to avoid leaving "zombie" processes.
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// handle SIGINT (interrupt, Ctrl+C) signal
void handle_sigint(int signo)
{
    if (foreground != -1)  // when a program is running
    {
        // send SIGINT to the foreground process
        kill(foreground, SIGINT);

        foreground = -1; // reset foreground process ID
        printf("\n");
        fflush(stdout); // flush the output buffer
    }
    else
    {
        // when a program is not running
        fprintf(stdout, "\n");
        rl_replace_line("", 0);     // clear previous input line
        rl_forced_update_display(); // force readline to update the display
    }
}

// handle SIGTSTP (terminal stop, Ctrl+Z) signal
void handle_sigtstp(int signo)
{
    if (foreground != -1)  // when a program is running
    {
        // send SIGTSTP to the foreground process
        kill(foreground, SIGTSTP);

        foreground = -1; // reset foreground process ID
        printf("\nStopped");
        printf("\n");
        fflush(stdout); // flush the output buffer
    }
    else
    {
        // when a program is not running
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

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

// Set up signal handlers
void setup_signal_handlers()
{
    // Ctrl+C
    signal(SIGINT, handle_sigint);
    // Ctrl+Z
    signal(SIGTSTP, handle_sigtstp);
    // Child process terminated
    signal(SIGCHLD, handle_sigchld);
}

// Handle SIGCHLD (Child process terminated) signal
void handle_sigchld(int signo)
{
    // Reap all terminated child processes to avoid leaving "zombie" processes.
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Handle SIGINT (interrupt, Ctrl+C) signal
void handle_sigint(int signo)
{
    if (foreground != -1)  // When a program is running
    {
        // Send SIGINT to the foreground process
        kill(foreground, SIGINT);

        foreground = -1; // Reset foreground process ID
        printf("\n");
        fflush(stdout); // Flush the output buffer
    }
    else
    {
        // When a program is not running
        fprintf(stdout, "\n");
        rl_replace_line("", 0);     // Clear previous input line
        rl_forced_update_display(); // Force readline to update the display
    }
}

// Handle SIGTSTP (terminal stop, Ctrl+Z) signal
void handle_sigtstp(int signo)
{
    if (foreground != -1)  // When a program is running
    {
        // Send SIGTSTP to the foreground process
        kill(foreground, SIGTSTP);

        foreground = -1; // Reset foreground process ID
        printf("\nStopped");
        printf("\n");
        fflush(stdout); // Flush the output buffer
    }
    else
    {
        // When a program is not running
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

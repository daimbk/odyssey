#include "job_control.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "prompt.h"

int next_job_id = 1;
pid_t foreground = -1;
volatile int exit_flag = 0;

Node *head = NULL;

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
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	pid_t pid;
	int status;

	// Reap all terminated child processes to avoid leaving "zombie" processes
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		if (WIFEXITED(status) || WIFSIGNALED(status)) {
			// Print information about the terminated background process
			printf("[%d] Done\t%s\n", pid, get_command_from_pid(pid));
		}

		pid_t terminated_pid;
		while ((terminated_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
			remove_job(terminated_pid);
		}
	}
}

// handle SIGINT (interrupt, Ctrl+C) signal
void handle_sigint(int signo)
{
	if (foreground != -1)  // when a program is running
	{
		// send SIGINT to the foreground process
		kill(foreground, SIGINT);

		foreground = -1;  // reset foreground process ID
		printf("\n");
		fflush(stdout);	 // flush the output buffer
	} else {
		// when a program is not running
		fprintf(stdout, "\n");
		rl_replace_line("", 0);	     // clear previous input line
		rl_forced_update_display();  // force readline to update the display
	}
	exit_flag = 1;
}

// handle SIGTSTP (terminal stop, Ctrl+Z) signal
void handle_sigtstp(int signo)
{
	if (foreground != -1)  // when a program is running
	{
		// send SIGTSTP to the foreground process
		kill(foreground, SIGTSTP);

		foreground = -1;  // reset foreground process ID
		printf("\nStopped");
		printf("\n");
		fflush(stdout);	 // flush the output buffer
	} else {
		// when a program is not running
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	exit_flag = 1;
}

void add_job(pid_t pid, const char *command)
{
	Node *new_node = (Node *)malloc(sizeof(Node));
	if (!new_node) {
		perror("Error: Unable to allocate memory for job information");
		exit(EXIT_FAILURE);
	}

	new_node->pid = pid;
	new_node->command = strdup(command);  // strdup allocates memory for the string
	new_node->next = head;

	head = new_node;
}

const char *get_command_from_pid(pid_t pid)
{
	Node *current = head;

	while (current != NULL) {
		if (current->pid == pid) {
			return current->command;
		}

		current = current->next;
	}

	return "Unknown Command";
}

int get_next_job_id() { return next_job_id++; }

void remove_job(pid_t pid)
{
	Node *current = head;
	Node *prev = NULL;

	while (current != NULL) {
		if (current->pid == pid) {
			if (prev == NULL) {
				// The node to be removed is the head
				head = current->next;
			} else {
				prev->next = current->next;
			}

			free(current->command);
			free(current);
			return;
		}

		prev = current;
		current = current->next;
	}
}

void print_jobs()
{
	Node *current = head;

	if (current == NULL) {
		printf("No background jobs.\n");
		return;
	}

	printf("Background jobs:\n");
	while (current != NULL) {
		printf("[%d] %d\t%s\n", get_next_job_id(), current->pid, current->command);
		current = current->next;
	}
}

void cleanup_jobs()
{
	Node *current = head;

	while (current != NULL) {
		Node *temp = current;
		current = current->next;
		free(temp->command);
		free(temp);
	}

	head = NULL;
}
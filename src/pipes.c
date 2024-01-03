#include "pipes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_pipe(char **tokens, int pipeIndex, int tokenCount)
{
	int pipe_fd[2];
	pid_t child_pid1, child_pid2;

	// create a pipe
	if (pipe(pipe_fd) == -1) {
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}

	// create the first child process
	if ((child_pid1 = fork()) == -1) {
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}

	if (child_pid1 == 0) {
		// close the read end of the pipe
		close(pipe_fd[0]);

		// redirect standard output to the write end of the pipe
		dup2(pipe_fd[1], STDOUT_FILENO);

		// close the write end of the pipe
		close(pipe_fd[1]);

		// null-terminate the tokens array at the pipe character
		tokens[pipeIndex] = NULL;

		// execute the command before the pipe
		if (execvp(tokens[0], tokens) == -1) {
			perror("Command execution failed");
			exit(EXIT_FAILURE);
		}
	}

	// create the second child process
	if ((child_pid2 = fork()) == -1) {
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}

	if (child_pid2 == 0) {
		// close the write end of the pipe
		close(pipe_fd[1]);

		// redirect standard input to the read end of the pipe
		dup2(pipe_fd[0], STDIN_FILENO);

		// close the read end of the pipe
		close(pipe_fd[0]);

		// rxecute the command after the pipe
		if (execvp(tokens[pipeIndex + 1], &tokens[pipeIndex + 1]) == -1) {
			perror("Command execution failed");
			exit(EXIT_FAILURE);
		}
	}

	// close both ends of the pipe in the parent
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	// wait for both child processes to finish
	waitpid(child_pid1, NULL, 0);
	waitpid(child_pid2, NULL, 0);
}

// find the index of the first pipe symbol in the tokens array
int findPipe(char **tokens, int tokenCount)
{
	for (int i = 0; i < tokenCount; i++) {
		if (strcmp(tokens[i], "|") == 0) {
			return i;
		}
	}
	return -1;
}

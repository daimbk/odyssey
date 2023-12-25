#include "redirection.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

void handle_redirection(char **tokens, int redirIndex, int tokenCount)
{
    int fd;
    pid_t child_pid;

    // determine the type of redirection
    char *redir_type = tokens[redirIndex];

    // open file for redirection
    if (redir_type[0] == '>')
    {
        fd = open(tokens[redirIndex + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else if (redir_type[0] == '<')
    {
        fd = open(tokens[redirIndex + 1], O_RDONLY);
    }

    if (fd == -1)
    {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    // create a child process
    if ((child_pid = fork()) == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        // redirect standard input/output to the file
        dup2(fd, (redir_type[0] == '>') ? STDOUT_FILENO : STDIN_FILENO);

        // close the file descriptor
        close(fd);

        // null-terminate the tokens array at the redirection character
        tokens[redirIndex] = NULL;

        // execute the command before/after the redirection
        if (execvp(tokens[0], tokens) == -1)
        {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file descriptor in the parent
    close(fd);

    // Wait for the child process to finish
    waitpid(child_pid, NULL, 0);
}

// find the index of the first redirection symbol in the tokens array
int findRedirection(char **tokens, int tokenCount)
{
    for (int i = 0; i < tokenCount; i++)
    {
        if (strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], "<") == 0)
        {
            return i;
        }
    }
    return -1;
}

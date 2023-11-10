#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "directory.h"
#include "prompt.h"

#define MAX_INPUT_SIZE 1024

int main()
{
    char input[MAX_INPUT_SIZE];
    char currentDir[PATH_MAX];
    char hostName[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];

    while (1)
    {
        getPromptInfo(username, hostName, currentDir);
        printf("%s%s@%s:%s%s$ %s", COLOR_LIGHT_PURPLE, username, hostName, COLOR_CYAN, currentDir, COLOR_RESET);

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
        {
            break;
        }

        // remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        // declare an array to store tokens
        char *tokens[MAX_INPUT_SIZE];

        // tokenize input
        char *token = strtok(input, " ");
        int tokenCount = 0; // to keep track of the number of tokens

        while (token != NULL)
        {
            // store the token in the tokens array
            tokens[tokenCount] = token;
            tokenCount++;

            token = strtok(NULL, " ");
        }

        // handle commands

        // quit shell on "exit" command
        if (tokenCount == 1 && strcmp(tokens[0], "exit") == 0)
        {
            printf("Exiting Shell..\n");
            exit(0);
        }

        if (tokenCount > 0)
        {
            if (strcmp(tokens[0], "cd") == 0)
            {
                if (tokenCount < 2)
                {
                    if (changeDirectory(getenv("HOME")) != 0)
                    {
                        perror("Error: cd");
                    }
                }
                else
                {
                    if (changeDirectory(tokens[1]) != 0)
                    {
                        perror("Error: cd");
                    }
                }
            }
            else
            {
                // handle other commands using execvp
                // create a child process
                pid_t child_pid = fork();

                if (child_pid == -1)
                {
                    perror("Error: Fork failed");
                    exit(EXIT_FAILURE);
                }

                if (child_pid == 0)
                {
                    // Child process

                    // Null-terminate the tokens array
                    tokens[tokenCount] = NULL;

                    if (execvp(tokens[0], tokens) == -1)
                    {
                        perror("Error executing command");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // Parent process

                    // wait for the child to finish
                    int status;
                    waitpid(child_pid, &status, 0);
                }
            }
        }
    }

    return 0;
}

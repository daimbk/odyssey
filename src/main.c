#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "directory.h"
#include "prompt.h"
#include "system_commands.h"

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
            else if (strcmp(tokens[0], "mkdir") == 0)
            {
                if (tokenCount < 2)
                {
                    // Handle error, not enough arguments
                    printf("Usage: mkdir <directory_name>\n");
                }
                else
                {
                    if (createDirectory(tokens[1]) != 0)
                    {
                        perror("Error: mkdir");
                    }
                }
            }
            else if (strcmp(tokens[0], "rmdir") == 0)
            {
                if (tokenCount < 2)
                {
                    // Handle error, not enough arguments
                    printf("Usage: rmdir <directory_name>\n");
                }
                else
                {
                    if (deleteDirectory(tokens[1]) != 0)
                    {
                        perror("Error: rmdir");
                    }
                }
            }
            else if (strcmp(tokens[0], "ps") == 0)
            {
                ps_command();
            }
            else if (strcmp(tokens[0], "kill") == 0)
            {
                // check if there are enough arguments
                if (tokenCount < 3)
                {
                    printf("Usage: kill <pid> <signal>\n");
                }
                else
                {
                    // extract PID and signal from user input
                    int pid = atoi(tokens[1]);
                    int signal = atoi(tokens[2]);

                    kill_command(pid, signal);
                }
            }
            else
            {
                // Handle other commands using execvp
                // Create a child process
                pid_t child_pid = fork();

                if (child_pid == -1)
                {
                    perror("Error: Fork Failed");
                    exit(1);
                }

                if (child_pid == 0)
                {
                    // Child process

                    // Null-terminate the tokens array
                    tokens[tokenCount] = NULL;

                    if (execvp(tokens[0], tokens) == -1)
                    {
                        perror("Error: execvp");
                        exit(1);
                    }
                }
                else
                {
                    // Parent process

                    // Wait for the child to finish
                    int status;
                    waitpid(child_pid, &status, 0);
                }
            }
        }
    }

    return 0;
}

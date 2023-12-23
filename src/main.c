#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "directory.h"
#include "prompt.h"
#include "autocomplete.h"
#include "compression.h"

#define MAX_INPUT_SIZE 1024

int main()
{
    char currentDir[PATH_MAX];
    char hostName[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];

    initializeHistory(); // Initialize command history

    while (1)
    {
        getPromptInfo(username, hostName, currentDir);
        char *prompt = malloc(MAX_INPUT_SIZE + 50); // Add extra space for the prompt
        sprintf(prompt, "%s%s@%s:%s%s$ %s", COLOR_LIGHT_PURPLE, username, hostName, COLOR_CYAN, currentDir, COLOR_RESET);

        rl_attempted_completion_function = (rl_completion_func_t *)autocomplete_generator;

        char *input = readline(prompt); // Use readline for input
        free(prompt);

        if (input == NULL)
        {
            break;
        }

        // If the input is not empty, add it to history
        if (strlen(input) > 0) {
            add_history(input);
        }

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
            free(input);
            break;
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
            else if (strcmp(tokens[0], "cp") == 0)
            {
                if (tokenCount < 3)
                {
                    printf("Usage: cp <source> <destination>\n");
                }
                else
                {
                    if (copyFile(tokens[1], tokens[2]) != 0)
                    {
                        perror("Error: cp");
                    }
                }
            }
            else if (strcmp(tokens[0], "mv") == 0)
            {
                if (tokenCount < 3)
                {
                    printf("Usage: mv <source> <destination>\n");
                }
                else
                {
                    if (moveFile(tokens[1], tokens[2]) != 0)
                    {
                        perror("Error: mv");
                    }
                }
            }
            else if (strcmp(tokens[0], "rm") == 0)
            {
                if (tokenCount < 2)
                {
                    printf("Usage: rm <file_name>\n");
                }
                else
                {
                    if (deleteFile(tokens[1]) != 0)
                    {
                        perror("Error: rm");
                    }
                }
            }
            else if (strcmp(tokens[0], "rename") == 0)
            {
                if (tokenCount < 3)
                {
                    printf("Usage: rename <old_name> <new_name>\n");
                }
                else
                {
                    if (renameFile(tokens[1], tokens[2]) != 0)
                    {
                        perror("Error: mv");
                    }
                }
            }
            else if (strcmp(tokens[0], "stat") == 0)
            {
                if (tokenCount < 2)
                {
                    printf("Usage: stat <file_or_directory>\n");
                }
                else
                {
                    if (getFileInformation(tokens[1]) != 0)
                    {
                        perror("Error: stat");
                    }
                }
            }
            else if (strcmp(tokens[0], "gzip") == 0)
            {
                if (tokenCount < 2)
                {
                    printf("Usage: gzip <file_name>\n");
                }
                else
                {
                    if (compressFile(tokens[1]) != 0)
                    {
                        perror("Error: gzip");
                    }
                }
            }
            else if (strcmp(tokens[0], "gunzip") == 0)
            {
                if (tokenCount < 2)
                {
                    printf("Usage: gunzip <file_name.gz>\n");
                }
                else
                {
                    if (decompressFile(tokens[1]) != 0)
                    {
                        perror("Error: gunzip");
                    }
                }
            }
            else if (strcmp(tokens[0], "exit") == 0)
            {
                printf("Exiting Shell..\n");
                free(input);
                break;
            }
            else
            {
                // Handle other commands using execvp
                // Create a child process
                pid_t child_pid = fork();

                if (child_pid == -1)
                {
                    perror("Error: Fork Failed");
                    free(input);
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
                        free(input);
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

        free(input);
    }

    return 0;
}

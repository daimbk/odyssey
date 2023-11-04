#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

int main()
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        printf("da-shell:~$ ");
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

        // quit shell on "exit" command
        if (tokenCount == 1 && strcmp(tokens[0], "exit") == 0)
        {
            printf("Exiting Shell..\n");
            exit(0);
        }

        // create a child process
        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            perror("Error: Fork Failed");
            exit(1);
        }

        if (child_pid == 0)
        {
            // child process

            // null-terminate the tokens array
            tokens[tokenCount] = NULL;

            if (execvp(tokens[0], tokens) == -1)
            {
                perror("Error: execvp");
                exit(1);
            }
        }
        else
        {
            // parent process

            // wait for the child to finish
            int status;
            waitpid(child_pid, &status, 0);
        }
    }

    return 0;
}

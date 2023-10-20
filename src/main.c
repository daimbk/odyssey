#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

        // tokenize input
        // Process input (executing commands, etc.)
        // for example, currently displaying the parsed tokens as output
        char *token = strtok(input, " ");
        while (token != NULL)
        {
            // quit shell on "exit" command
            if (strcmp(token, "exit") == 0)
            {
                printf("Exiting Shell..\n");
                exit(0);
            }

            printf("Token: %s\n", token);
            token = strtok(NULL, " ");
        }
    }

    return 0;
}

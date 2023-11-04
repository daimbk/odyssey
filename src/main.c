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

        // printing tokens for testing
        for (int i = 0; i < tokenCount; i++)
        {
            printf("Token %d: %s\n", i, tokens[i]);
        }
    }

    return 0;
}

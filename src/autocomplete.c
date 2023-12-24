#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "prompt.h"
#include "autocomplete.h"

static char *completions[] = {
    "ls",
    "cd",
    "mkdir",
    "rmdir",
    "exit",
    // Add more completions as needed
    NULL // Mark the end of the list
};

char *get_completion(int index)
{
    if (completions[index] != NULL)
    {
        return completions[index];
    }
    else
    {
        return NULL;
    }
}

char *autocomplete_generator(const char *text, int state)
{
    static int list_index, len;
    char *name;

    // On the first call, initialize variables
    if (!state)
    {
        list_index = 0;
        len = strlen(text);
    }

    // Iterate through your list of possible completions
    while ((name = get_completion(list_index++)) != NULL)
    {
        if (strncmp(name, text, len) == 0)
        {
            // Return the matched completion
            return strdup(name);
        }
    }

    // No more completions
    return NULL;
}

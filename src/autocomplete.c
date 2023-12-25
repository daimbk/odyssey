#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "autocomplete.h"
#include "prompt.h"

char **autocomplete_generator(const char *text, int start, int end)
{
    // NULL-terminate the input string for compatibility
    char *partialInput = NULL;
    if (start == 0)
    {
        partialInput = strdup("");
    }
    else
    {
        partialInput = strndup(text, end);
    }

    // Get the matches based on the partial input
    char **matches = NULL;
    //printf("Partial Input: \"%s\"\n", partialInput);

    // Customize this part to provide more dynamic completion suggestions
    if (strcmp(partialInput, "") != 0)
    {
        // Suggest commands from history
        matches = rl_completion_matches(partialInput, (rl_compentry_func_t *)getCommandFromHistory);
    }
    else
    {
        // Suggest files and directories from the current path
        matches = rl_completion_matches(partialInput, (rl_compentry_func_t *)rl_filename_completion_function);
    }

    // Clean up
    free(partialInput);

    return matches;
}

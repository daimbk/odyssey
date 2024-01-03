#include "autocomplete.h"

#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prompt.h"

char **autocomplete_generator(const char *text, int start, int end)
{
	// NULL-terminate the input string for compatibility
	char *partialInput = NULL;
	if (start == 0) {
		partialInput = strdup("");
	} else {
		partialInput = strndup(text, end);
	}

	// get the matches based on the partial input
	char **matches = NULL;
	// printf("Partial Input: \"%s\"\n", partialInput);

	// customize this part to provide more dynamic completion suggestions
	if (strcmp(partialInput, "") != 0) {
		// suggest commands from history
		matches = rl_completion_matches(partialInput, (rl_compentry_func_t *)getCommandFromHistory);
	} else {
		// suggest files and directories from the current path
		matches = rl_completion_matches(partialInput, (rl_compentry_func_t *)rl_filename_completion_function);
	}

	free(partialInput);

	return matches;
}

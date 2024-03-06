#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autocomplete.h"
#include "commands.h"
#include "config.h"
#include "directory.h"
#include "job_control.h"
#include "prompt.h"

int main()
{
	// initialization
	loadConfig();
	setup_signal_handlers();
	initializeHistory();
	rl_attempted_completion_function = (rl_completion_func_t *)autocomplete_generator;
	displayASCII();

	char currentDir[PATH_MAX];
	char hostName[HOST_NAME_MAX];
	char username[LOGIN_NAME_MAX];

	while (1) {
		// prompt
		getPromptInfo(username, hostName, currentDir);
		char *prompt = malloc(MAX_INPUT_SIZE + 50);
		sprintf(prompt, "%s%s@%s%s:%s%s$ %s", usernameColor, username, hostnameColor, hostName, currentDirColor, currentDir, RESET);

		char *input = readline(prompt);
		free(prompt);

		if (input == NULL) {
			break;
		}

		// add input to history
		if (strlen(input) > 0) {
			add_history(input);
		}

		// tokenize input
		char *tokens[MAX_INPUT_SIZE];
		int tokenCount = tokenizeInput(input, tokens);

		// parse and handle command
		enum Command command = parseCommand(tokens[0]);
		handleCommand(command, tokens, tokenCount);

		free(input);
	}

	cleanup_jobs();
	return 0;
}

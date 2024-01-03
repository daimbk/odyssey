#ifndef PROMPT_H
#define PROMPT_H

// PATH_MAX definition in globals.h
#define HOST_NAME_MAX 64
#define LOGIN_NAME_MAX 256

#define COLOR_LIGHT_PURPLE "\033[38;5;55m"
#define COLOR_CYAN "\033[38;5;6m"
#define COLOR_RESET "\033[0m"

#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>

#include "globals.h"

void displayASCII();
void getPromptInfo(char *username, char *hostname, char *currentDir);
void replaceHomeWithPath(char *path);
char *getCommandFromHistory(int index);
void initializeHistory();

#endif

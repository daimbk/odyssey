#ifndef PROMPT_H
#define PROMPT_H

// PATH_MAX definition in globals.h
#define HOST_NAME_MAX 64
#define LOGIN_NAME_MAX 256

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
void hehe();

#endif

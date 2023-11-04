#ifndef PROMPT_H
#define PROMPT_H

#define PATH_MAX 4096
#define HOST_NAME_MAX 64
#define LOGIN_NAME_MAX 256

#define COLOR_DARK_PURPLE "\033[38;5;53m"
#define COLOR_TEAL "\033[38;5;30m"
#define COLOR_RESET "\033[0m"

#include <pwd.h>

void getPromptInfo(char *username, char *hostname, char *currentDir);
void replaceHomeWithPath(char *path);

#endif

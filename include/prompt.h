#ifndef PROMPT_H
#define PROMPT_H

#define PATH_MAX 4096
#define HOST_NAME_MAX 64
#define LOGIN_NAME_MAX 256

#define COLOR_LIGHT_PURPLE "\033[38;5;55m"
#define COLOR_CYAN "\033[38;5;6m"
#define COLOR_RESET "\033[0m"

#include <pwd.h>

void getPromptInfo(char *username, char *hostname, char *currentDir);
void replaceHomeWithPath(char *path);

#endif

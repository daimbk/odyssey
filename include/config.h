#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include "globals.h"

#define CONFIG_FOLDER_NAME ".odyssey"
#define CONFIG_FILE_NAME "shellconfig.txt"

// -------CONFIG VARIABLES-------
extern bool show_ascii_art;
extern char *usernameColor;
extern char *hostnameColor;
extern char *currentDirColor;

void createConfigFolder();
void loadConfig();
void toggle_ascii_art(char *cmd);

void setUsernameColor(char *color);
void setHostnameColor(char *color);
void setCurrentDirColor(char *color);

#endif

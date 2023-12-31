#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FOLDER_NAME ".odyssey"
#define CONFIG_FILE_NAME "shellconfig.txt"

#include <stdbool.h>

// -------CONFIG VARIABLES-------
extern bool show_ascii_art;

void createConfigFolder();
void loadConfig();

#endif

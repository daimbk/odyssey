#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool show_ascii_art = false;
char *usernameColor = PURPLE;
char *hostnameColor = CYAN;
char *currentDirColor = WHITE;

// function to create and write default config
void createDefaultConfig(char *configFilePath)
{
	// create and open the config file
	FILE *configFile = fopen(configFilePath, "w");
	if (configFile == NULL) {
		perror("Error creating config file");
		exit(EXIT_FAILURE);
	}

	// --------WRITE DEFAULT CONFIG TO FILE-------
	fprintf(configFile, "show_ascii_art=true\n");

	// close the file
	fclose(configFile);
}

void createConfigFolder()
{
	char homeDir[1024];

	// get the home directory path
	const char *homeEnv = getenv("HOME");

	if (homeEnv == NULL) {
		fprintf(stderr, "Error: HOME environment variable not set.\n");
		exit(EXIT_FAILURE);
	}

	strcpy(homeDir, homeEnv);

	// create the full path for the config folder
	char configFolderPath[strlen(homeDir) + 1 + strlen(CONFIG_FOLDER_NAME) + 1];
	snprintf(configFolderPath, sizeof(configFolderPath), "%s/%s", homeDir, CONFIG_FOLDER_NAME);

	// create the full path for the config file
	char configFilePath[strlen(configFolderPath) + 1 + strlen(CONFIG_FILE_NAME) + 1];
	snprintf(configFilePath, sizeof(configFilePath), "%s/%s", configFolderPath, CONFIG_FILE_NAME);

	// check if the folder already exists, create it if not
	struct stat st = {0};
	if (stat(configFolderPath, &st) == -1) {
		if (mkdir(configFolderPath, 0700) != 0) {
			perror("Error creating config folder");
			exit(EXIT_FAILURE);
		}

		createDefaultConfig(configFilePath);
	} else {
		// folder exists, check if config file exists
		if (stat(configFilePath, &st) == -1) {
			createDefaultConfig(configFilePath);
		}
	}
}

void loadConfig()
{
	char *home_dir = getenv("HOME");
	char config_path[strlen(home_dir) + 1 + strlen(CONFIG_FOLDER_NAME) + 1 + strlen(CONFIG_FILE_NAME) + 1];

	snprintf(config_path, sizeof(config_path), "%s/%s/%s", home_dir, CONFIG_FOLDER_NAME, CONFIG_FILE_NAME);

	FILE *fp = fopen(config_path, "r");

	if (fp == NULL) {
		createConfigFolder();
		fp = fopen(config_path, "r");
	}

	// ---------LOAD CONFIG FROM FILE----------
	char setting[64];
	char line[128];

	while (fgets(line, sizeof(line), fp)) {
		if (strstr(line, "show_ascii_art")) {
			sscanf(line, "show_ascii_art=%s", setting);

			if (strcmp(setting, "true") == 0) {
				show_ascii_art = true;
			} else {
				show_ascii_art = false;
			}
		}
	}

	fclose(fp);
}

void toggle_ascii_art(char *cmd)
{
	char *home = getenv("HOME");
	char config_file[256];
	strcpy(config_file, home);
	strcat(config_file, "/.odyssey/shellconfig.txt");

	FILE *fp = fopen(config_file, "r+");

	char line[64];
	while (fgets(line, sizeof(line), fp)) {
		if (strstr(line, "show_ascii_art")) {
			fseek(fp, -strlen(line), SEEK_CUR);
			fputs("show_ascii_art=", fp);

			if (strcmp(cmd, "enable") == 0) {
				fputs("true\n", fp);
			} else {
				fputs("false\n", fp);
			}
			break;
		}
	}

	fclose(fp);
}

void setColor(char *color, char **variable, const char *name)
{
	if (strcmp(color, "RESET") == 0) {
		*variable = RESET;
	} else if (strcmp(color, "BLACK") == 0) {
		*variable = BLACK;
	} else if (strcmp(color, "RED") == 0) {
		*variable = RED;
	} else if (strcmp(color, "GREEN") == 0) {
		*variable = GREEN;
	} else if (strcmp(color, "YELLOW") == 0) {
		*variable = YELLOW;
	} else if (strcmp(color, "BLUE") == 0) {
		*variable = BLUE;
	} else if (strcmp(color, "MAGENTA") == 0) {
		*variable = MAGENTA;
	} else if (strcmp(color, "WHITE") == 0) {
		*variable = WHITE;
	} else if (strcmp(color, "PURPLE") == 0) {
		*variable = PURPLE;
	} else if (strcmp(color, "LIGHT_PURPLE") == 0) {
		*variable = LIGHT_PURPLE;
	} else if (strcmp(color, "DARK_PURPLE") == 0) {
		*variable = DARK_PURPLE;
	} else if (strcmp(color, "CYAN") == 0) {
		*variable = CYAN;
	} else {
		printf("Invalid color entered for %s. Keeping the default color.\n", name);
	}
}

void setUsernameColor(char *color) { setColor(color, &usernameColor, "username"); }
void setHostnameColor(char *color) { setColor(color, &hostnameColor, "hostname"); }
void setCurrentDirColor(char *color) { setColor(color, &currentDirColor, "current directory"); }

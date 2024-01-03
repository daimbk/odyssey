#include "compression.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

int compressFile(const char *filePath)
{
	char command[PATH_MAX + 20];
	snprintf(command, sizeof(command), "gzip -f %s", filePath);

	if (system(command) == -1) {
		perror("Error: system");
		return -1;
	}

	return 0;
}

int decompressFile(const char *filePath)
{
	char command[PATH_MAX + 24];
	snprintf(command, sizeof(command), "gunzip -f %s", filePath);

	if (system(command) == -1) {
		perror("Error: system");
		return -1;
	}

	return 0;
}

int compressFolder(const char *folderPath, const char *zipFilePath)
{
	char command[PATH_MAX + PATH_MAX + 50];
	snprintf(command, sizeof(command), "zip -r %s %s", zipFilePath, folderPath);

	if (system(command) == -1) {
		perror("Error: system");
		return -1;
	}

	return 0;
}

int decompressZip(const char *zipFilePath, const char *extractPath)
{
	char command[PATH_MAX + PATH_MAX + 50];
	snprintf(command, sizeof(command), "unzip -d %s %s", extractPath, zipFilePath);

	if (system(command) == -1) {
		perror("Error: system");
		return -1;
	}

	return 0;
}

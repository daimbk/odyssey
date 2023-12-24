#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#include "globals.h"
#include "compression.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int compressFile(const char *filePath)
{
    char command[PATH_MAX + 20]; // Maximum command length: "gzip -f filename"
    snprintf(command, sizeof(command), "gzip -f %s", filePath);

    execl("/bin/sh", "/bin/sh", "-c", command, (char *)NULL);
    perror("Error: exec");
    exit(EXIT_FAILURE);
}

int decompressFile(const char *filePath)
{
    char command[PATH_MAX + 24]; // Maximum command length: "gunzip -f filename.gz"
    snprintf(command, sizeof(command), "gunzip -f %s", filePath);

    execl("/bin/sh", "/bin/sh", "-c", command, (char *)NULL);
    perror("Error: exec");
    exit(EXIT_FAILURE);
}

int compressFolder(const char *folderPath, const char *zipFilePath)
{
    char command[PATH_MAX + PATH_MAX + 50]; // Maximum command length: "zip -r zipFileName folderPath"
    snprintf(command, sizeof(command), "zip -r %s %s", zipFilePath, folderPath);

    execl("/bin/sh", "/bin/sh", "-c", command, (char *)NULL);
    perror("Error: exec");
    exit(EXIT_FAILURE);
}

int decompressZip(const char *zipFilePath, const char *extractPath)
{
    char command[PATH_MAX + PATH_MAX + 50]; // Maximum command length: "unzip -d extractPath zipFileName"
    snprintf(command, sizeof(command), "unzip -d %s %s", extractPath, zipFilePath);

    execl("/bin/sh", "/bin/sh", "-c", command, (char *)NULL);
    perror("Error: exec");
    exit(EXIT_FAILURE);
}

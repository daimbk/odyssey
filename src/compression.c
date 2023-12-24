#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#include "globals.h"
#include "compression.h"

int executeCommand(const char *command)
{
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("Error: Fork Failed");
        return -1;
    }

    if (child_pid == 0)
    {
        // Child process
        execl("/bin/sh", "/bin/sh", "-c", command, (char *)NULL);
        perror("Error: exec");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        waitpid(child_pid, &status, 0);
        return status;
    }
}

int compressFile(const char *filePath)
{
    char command[PATH_MAX + 20]; // Maximum command length: "gzip -f filename"
    snprintf(command, sizeof(command), "gzip -f %s", filePath);

    return executeCommand(command);
}

int decompressFile(const char *filePath)
{
    char command[PATH_MAX + 24]; // Maximum command length: "gunzip -f filename.gz"
    snprintf(command, sizeof(command), "gunzip -f %s", filePath);

    return executeCommand(command);
}

int compressFolder(const char *folderPath, const char *zipFilePath)
{
    char command[PATH_MAX + PATH_MAX + 50]; // Maximum command length: "zip -r zipFileName folderPath"
    snprintf(command, sizeof(command), "zip -r %s %s", zipFilePath, folderPath);

    return executeCommand(command);
}

int decompressZip(const char *zipFilePath, const char *extractPath)
{
    char command[PATH_MAX + PATH_MAX + 50]; // Maximum command length: "unzip -d extractPath zipFileName"
    snprintf(command, sizeof(command), "unzip -d %s %s", extractPath, zipFilePath);

    return executeCommand(command);
}

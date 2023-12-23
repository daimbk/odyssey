#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <limits.h>

#include "directory.h"

int changeDirectory(const char *path)
{
    if (chdir(path) != 0)
    {
        perror("Error: chdir");
        return -1;
    }
    return 0;
}

int createDirectory(const char *path)
{
    if (mkdir(path, 0777) != 0)
    {
        perror("Error: mkdir");
        return -1;
    }
    return 0;
}

int deleteDirectory(const char *path)
{
    if (rmdir(path) != 0)
    {
        perror("Error: rmdir");
        return -1;
    }
    return 0;
}

int deleteFile(const char *path)
{
    if (remove(path) != 0)
    {
        perror("Error: deleteFile");
        return -1;
    }
    return 0;
}

int recursiveDelete(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    char filePath[PATH_MAX];

    if ((dir = opendir(path)) == NULL)
    {
        perror("Error: opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(filePath, sizeof(filePath), "%s/%s", path, entry->d_name);

        if (isDirectory(filePath))
        {
            if (recursiveDelete(filePath) != 0)
            {
                closedir(dir);
                return -1;
            }
        }
        else
        {
            if (deleteFile(filePath) != 0)
            {
                closedir(dir);
                return -1;
            }
        }
    }

    closedir(dir);

    if (rmdir(path) != 0)
    {
        perror("Error: rmdir");
        return -1;
    }

    return 0;
}


// Function to check if the path is a directory
int isDirectory(const char *path)
{
    struct stat pathStat;
    if (stat(path, &pathStat) == 0 && S_ISDIR(pathStat.st_mode))
    {
        return 1;
    }
    return 0;
}

// Recursive function to copy files and directories
int copyRecursive(const char *sourcePath, const char *destinationPath)
{
    DIR *sourceDir;
    struct dirent *entry;

    if ((sourceDir = opendir(sourcePath)) == NULL)
    {
        perror("Error: opendir");
        return -1;
    }

    while ((entry = readdir(sourceDir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char sourceFile[PATH_MAX], destFile[PATH_MAX];
        snprintf(sourceFile, sizeof(sourceFile), "%s/%s", sourcePath, entry->d_name);
        snprintf(destFile, sizeof(destFile), "%s/%s", destinationPath, entry->d_name);

        if (isDirectory(sourceFile))
        {
            if (mkdir(destFile, 0777) != 0)
            {
                perror("Error: mkdir");
                closedir(sourceDir);
                return -1;
            }
            if (copyRecursive(sourceFile, destFile) != 0)
            {
                closedir(sourceDir);
                return -1;
            }
        }
        else
        {
            if (copyFile(sourceFile, destFile) != 0)
            {
                closedir(sourceDir);
                return -1;
            }
        }
    }

    closedir(sourceDir);
    return 0;
}


int copyFile(const char *sourcePath, const char *destinationPath)
{
    struct stat destStat;
    if (stat(destinationPath, &destStat) == 0 && S_ISDIR(destStat.st_mode))
    {
        // Destination is a directory, construct the new path
        char destFile[PATH_MAX];
        snprintf(destFile, sizeof(destFile), "%s/%s", destinationPath, basename(strdup(sourcePath)));

        if (isDirectory(sourcePath))
        {
            // If source is a directory, create a new directory in the destination
            if (mkdir(destFile, 0777) != 0)
            {
                perror("Error: mkdir");
                return -1;
            }
            if (copyRecursive(sourcePath, destFile) != 0)
            {
                return -1;
            }
        }
        else
        {
            // If source is a file, perform regular file copy
            if (copyFile(sourcePath, destFile) != 0)
            {
                return -1;
            }
        }
    }
    else
    {
        // Destination is a file
        int sourceFile, destFile;
        char buffer[4096];
        ssize_t bytesRead;

        sourceFile = open(sourcePath, O_RDONLY);
        if (sourceFile == -1)
        {
            perror("Error: open source file");
            return -1;
        }

        destFile = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (destFile == -1)
        {
            perror("Error: open destination file");
            close(sourceFile);
            return -1;
        }

        while ((bytesRead = read(sourceFile, buffer, sizeof(buffer))) > 0)
        {
            if (write(destFile, buffer, bytesRead) != bytesRead)
            {
                perror("Error: write to destination file");
                close(sourceFile);
                close(destFile);
                return -1;
            }
        }

        close(sourceFile);
        close(destFile);
    }

    return 0;
}

int moveFile(const char *sourcePath, const char *destinationPath)
{
    struct stat destStat;
    if (stat(destinationPath, &destStat) == 0 && S_ISDIR(destStat.st_mode))
    {
        // Destination is a directory, construct the new path
        char newDestination[PATH_MAX];
        snprintf(newDestination, sizeof(newDestination), "%s/%s", destinationPath, basename(strdup(sourcePath)));

        if (isDirectory(sourcePath))
        {
            // If source is a directory, create a new directory in the destination
            if (mkdir(newDestination, 0777) != 0)
            {
                perror("Error: mkdir");
                return -1;
            }
            if (copyRecursive(sourcePath, newDestination) != 0)
            {
                return -1;
            }
        }
        else
        {
            // If source is a file, perform regular file copy
            if (copyFile(sourcePath, newDestination) != 0)
            {
                return -1;
            }
        }

        if (deleteFile(sourcePath) != 0)
        {
            perror("Error: moveFile (delete)");
            return -1;
        }
    }
    else
    {
        // Destination is a file
        if (rename(sourcePath, destinationPath) != 0)
        {
            perror("Error: moveFile");
            return -1;
        }
    }

    return 0;
}

int renameFile(const char *oldPath, const char *newPath)
{
    if (rename(oldPath, newPath) != 0)
    {
        perror("Error: renameFile");
        return -1;
    }
    return 0;
}

int getFileInformation(const char *path)
{
    struct stat fileStat;

    if (stat(path, &fileStat) == -1)
    {
        perror("Error: stat");
        return -1;
    }

    printf("File Information for %s:\n", path);
    printf("Size: %ld bytes\n", fileStat.st_size);
    printf("Permissions: %o\n", fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    // Add more information as needed

    return 0;
}
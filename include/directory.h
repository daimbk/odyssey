#ifndef DIRECTORY_H
#define DIRECTORY_H

int changeDirectory(const char *path);
int createDirectory(const char *path);
int deleteDirectory(const char *path);
int deleteFile(const char *path);
int recursiveDelete(const char *path);
int isDirectory(const char *path);
int copyRecursive(const char *sourcePath, const char *destinationPath);
int copyFile(const char *sourcePath, const char *destinationPath);
int moveFile(const char *sourcePath, const char *destinationPath);
int renameFile(const char *oldPath, const char *newPath);
int getFileInformation(const char *path);

#endif

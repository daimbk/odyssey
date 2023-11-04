#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "prompt.h"

void getPromptInfo(char *username, char *hostname, char *currentDir)
{
    struct passwd *pw = getpwuid(getuid());

    if (gethostname(hostname, HOST_NAME_MAX) != 0)
    {
        perror("Error: gethostname");
    }

    if (getcwd(currentDir, PATH_MAX) == NULL)
    {
        perror("Error: getcwd");
    }

    replaceHomeWithPath(currentDir);
    strncpy(username, pw->pw_name, strlen(pw->pw_name));
}

void replaceHomeWithPath(char *path)
{
    struct passwd *pw = getpwuid(getuid());
    const char *homeDir = pw->pw_dir;
    size_t homeDirLen = strlen(homeDir);

    if (strncmp(path, homeDir, homeDirLen) == 0)
    {
        memmove(path, "~", 1);
        memmove(path + 1, path + homeDirLen, strlen(path) - homeDirLen + 1);
    }
}

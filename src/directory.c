#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

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

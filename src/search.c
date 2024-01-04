#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "search.h"

int containsKeyword(const char *filename, const char *keyword) {
    return strstr(filename, keyword) != NULL;
}

void searchFiles(const char *directory, const char *keyword) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory)) == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (containsKeyword(entry->d_name, keyword) || strchr(keyword, 'k') != NULL) {
            printf("%s/%s\n", directory, entry->d_name);
        }

        if (entry->d_type == DT_DIR) {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
            searchFiles(path, keyword);
        }
    }

    closedir(dir);
}

// Implementation of the runFileSearch function
void runFileSearch(const char *directory, const char *keyword) {
    printf("Searching for files with keyword '%s' in directory '%s':\n", keyword, directory);
    searchFiles(directory, keyword);
    printf("Search complete.\n");
}

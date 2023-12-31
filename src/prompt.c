#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/ioctl.h>
#include "prompt.h"

// void displayASCII(bool isEnabled)
// {
//     if (isEnabled)
//     {
//         printf("\n\n");
//         printf("________ ________ ______.___. _________ __________________________.___.\n");
//         printf("\\_____  \\ \\______ \\__   |   |/   _____//   _____/\\_   _____/\\__   |   |\n");
//         printf(" /   |   \\ |    |  \\/   |   |\\_____  \\ \\_____  \\  |    ___)_  /   |   |\n");
//         printf("/    |    \\|    `   \\____   |/        \\/        \\ |         \\ \\____   |\n");
//         printf("\\_______  /_______  / ______/_______  /_______  / /_______  / / ______/\n");
//         printf("        \\/        \\/\\/              \\/        \\/          \\/  \\/       \n");
//         printf("\n\n");
//     }
// }

// print in horizontal center of terminal
void displayASCII(bool isEnabled)
{

    if (isEnabled)
    {

        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int term_cols = w.ws_col;

        int ascii_art_width = 76;
        int padding = (term_cols - ascii_art_width) / 2;

        printf("\n\n");
        printf("%*s________ ________ ______.___. _________ __________________________.___.\n", padding, "");
        printf("%*s\\_____  \\ \\______ \\__   |   |/   _____//   _____/\\_   _____/\\__   |   |\n", padding, "");
        printf("%*s /   |   \\ |    |  \\/   |   |\\_____  \\ \\_____  \\  |    ___)_  /   |   |\n", padding, "");
        printf("%*s/    |    \\|    `   \\____   |/        \\/        \\ |         \\ \\____   |\n", padding, "");
        printf("%*s\\_______  /_______  / ______/_______  /_______  / /_______  / / ______/\n", padding, "");
        printf("%*s        \\/        \\/\\/              \\/        \\/          \\/  \\/       \n", padding, "");
        printf("\n\n");
    }
}

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

char *getCommandFromHistory(int index)
{
    HIST_ENTRY *entry = history_get(index);
    if (entry)
    {
        return entry->line;
    }
    return NULL;
}

void initializeHistory()
{
    using_history();
}
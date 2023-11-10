#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define PATH_MAX_LENGTH 512

void ps_command()
{
    DIR *proc_dir;
    struct dirent *entry;

    // open the /proc directory
    if ((proc_dir = opendir("/proc")) == NULL)
    {
        perror("Error opening /proc directory");
        return;
    }

    // print header
    printf("%-8s %-20s %-10s\n", "PID", "Name", "Status");

    // read the /proc directory
    while ((entry = readdir(proc_dir)) != NULL)
    {
        // construct the full path
        char full_path[PATH_MAX_LENGTH];
        snprintf(full_path, sizeof(full_path), "/proc/%s", entry->d_name);

        // get information about the file
        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1)
        {
            perror("Error stat");
            continue;
        }

        // check if the entry is a directory and represents a process
        if (S_ISDIR(file_stat.st_mode) && atoi(entry->d_name) != 0)
        {
            // obtain process information from /proc/<pid>/status
            char status_path[PATH_MAX_LENGTH];
            snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);

            FILE *status_file = fopen(status_path, "r");
            if (status_file != NULL)
            {
                char line[256];
                char name[256];
                char status[256];

                // read the status file to get process name and status
                while (fgets(line, sizeof(line), status_file) != NULL)
                {
                    if (sscanf(line, "Name: %s", name) == 1)
                    {
                        continue;
                    }
                    else if (sscanf(line, "State: %s", status) == 1)
                    {
                        break;
                    }
                }

                // print process information
                printf("%-8s %-20s %-10s\n", entry->d_name, name, status);

                fclose(status_file);
            }
        }
    }

    // close the /proc directory
    closedir(proc_dir);
}

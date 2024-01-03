#include <processinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_process_info(char *pid)
{
	// file paths for various information
	char status_path[MAX_PATH_LEN];
	char cmdline_path[MAX_PATH_LEN];
	char fd_path[MAX_PATH_LEN];

	// construct file paths
	snprintf(status_path, MAX_PATH_LEN, "/proc/%s/status", pid);
	snprintf(cmdline_path, MAX_PATH_LEN, "/proc/%s/cmdline", pid);
	snprintf(fd_path, MAX_PATH_LEN, "/proc/%s/fd", pid);

	// display process ID
	printf("Process ID (PID): %s\n", pid);

	// read and display parent process ID
	char parent_pid[MAX_PATH_LEN];
	FILE *status_file = fopen(status_path, "r");
	if (status_file != NULL) {
		char line[MAX_PATH_LEN];
		while (fgets(line, sizeof(line), status_file) != NULL) {
			if (sscanf(line, "PPid:\t%s", parent_pid) == 1) {
				fclose(status_file);
				printf("Parent Process ID (PPID): %s\n", parent_pid);
				break;
			}
		}
	} else {
		perror("Error opening status file");
		exit(EXIT_FAILURE);
	}

	// read and display command line
	char cmdline[MAX_CMDLINE_LEN];
	FILE *cmdline_file = fopen(cmdline_path, "r");
	if (cmdline_file != NULL) {
		size_t bytesRead = fread(cmdline, 1, sizeof(cmdline) - 1, cmdline_file);
		if (bytesRead > 0) {
			cmdline[bytesRead] = '\0';  // ensure null-termination
			fclose(cmdline_file);
			printf("Command Line: %s\n", cmdline);
		}
	} else {
		perror("Error opening cmdline file");
		exit(EXIT_FAILURE);
	}

	// display memory utilization information from the status file
	printf("Memory Utilization Information:\n");
	system("cat /proc/$$/status | grep -E 'VmSize|VmRSS'");

	// display list of open files from the fd directory
	printf("List of Open Files:\n");
	system("ls -l /proc/$$/fd");
}

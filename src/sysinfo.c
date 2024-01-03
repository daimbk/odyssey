#include <job_control.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysinfo.h>
#include <unistd.h>

int display_system_info(const char *topN_str)
{
	int updateInterval = 5;	 // seconds
	int topN = atoi(topN_str);

	if (topN <= 0 || topN > MAX_PROCESSES) {
		fprintf(stderr, "Invalid number of processes. Please enter a value between 1 and %d\n", MAX_PROCESSES);
		return EXIT_FAILURE;
	}

	// set up signal handlers
	setup_signal_handlers();

	while (!exit_flag) {
		system("clear");

		// display system information
		printf("System Information:\n");
		displayMemoryInfo();
		displayCpuUsage();
		displayDiskUsage();
		displayTopProcesses(topN);

		// sleep for the specified update interval
		sleep(updateInterval);
	}
	exit_flag = 0;

	return 0;
}

// read and display total and free memory
void displayMemoryInfo()
{
	FILE *memInfo = fopen("/proc/meminfo", "r");
	if (memInfo != NULL) {
		char buffer[MAX_BUFFER_LEN];
		while (fgets(buffer, sizeof(buffer), memInfo) != NULL) {
			if (strncmp(buffer, "MemTotal:", 9) == 0 || strncmp(buffer, "MemFree:", 8) == 0) {
				printf("%s", buffer);
			}
		}
		fclose(memInfo);
	} else {
		perror("Error opening /proc/meminfo");
	}
}

// read and display CPU usage
void displayCpuUsage()
{
	FILE *statFile = fopen("/proc/stat", "r");
	if (statFile != NULL) {
		char buffer[MAX_BUFFER_LEN];
		if (fgets(buffer, sizeof(buffer), statFile) != NULL) {
			if (strncmp(buffer, "cpu ", 4) == 0) {
				unsigned long long user, nice, system, idle;
				sscanf(buffer + 4, "%llu %llu %llu %llu", &user, &nice, &system, &idle);

				unsigned long long totalCpuTime = user + nice + system + idle;
				unsigned long long idleTime = idle;

				double cpuUsage = ((totalCpuTime - idleTime) * 100.0) / totalCpuTime;
				printf("CPU Usage: %.2f%%\n", cpuUsage);
			}
		}
		fclose(statFile);
	} else {
		perror("Error opening /proc/stat");
	}
}

// read and display disk usage
void displayDiskUsage()
{
	FILE *df = popen("df -h /", "r");
	if (df != NULL) {
		char buffer[MAX_BUFFER_LEN];
		if (fgets(buffer, sizeof(buffer), df) != NULL) {
			printf("Disk Usage: %s", buffer);
		}
		pclose(df);
	} else {
		perror("Error running df command");
	}
}

// display the top N processes
void displayTopProcesses(int topN)
{
	printf("\n");
	printf("%-4s%-13s%-5s%-5s%-5s%-6s%-7s%-8s%-12s%-20s\n", "Rank", "USER", "PID", "%CPU", "%MEM", "VSZ", "RSS", "TTY", "STAT", "COMMAND");

	// run 'ps' command to get the top processes
	char psCommand[MAX_BUFFER_LEN];
	snprintf(psCommand, sizeof(psCommand), "ps aux --sort=-%%cpu,%%mem -ww | head -n+%d", topN + 6);

	FILE *ps = popen(psCommand, "r");
	if (ps != NULL) {
		char buffer[MAX_BUFFER_LEN];
		int counter = 1;  // counter for numbering processes
		while (fgets(buffer, sizeof(buffer), ps) != NULL) {
			if (counter > 5)  // skip the header lines
			{
				// extract and print relevant information with proper formatting
				char user[16], pid[8], cpu[8], mem[8], vsz[8], rss[8], tty[8], stat[8],
				    command[159];  // command[159] for beter formatting

				sscanf(buffer, "%s%s%s%s%9s%s%s%s", user, pid, cpu, mem, vsz, rss, tty, stat);

				// read the entire command line into the 'command' variable
				fgets(command, sizeof(command), ps);

				printf("%-4d%-20s", counter - 5, command);
			}
			counter++;
		}

		pclose(ps);
	} else {
		perror("Error running ps command");
	}
}

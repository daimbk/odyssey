#ifndef SYSINFO_H
#define SYSINFO_H

#define MAX_BUFFER_LEN 1024
#define MAX_PROCESSES 10

int display_system_info(const char *topN_str);
void displayMemoryInfo();
void displayCpuUsage();
void displayDiskUsage();
void displayTopProcesses(int topN);

#endif

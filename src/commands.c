#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "autocomplete.h"
#include "commands.h"
#include "compression.h"
#include "config.h"
#include "directory.h"
#include "job_control.h"
#include "pipes.h"
#include "processinfo.h"
#include "prompt.h"
#include "redirection.h"
#include "search.h"
#include "sysinfo.h"

int tokenizeInput(char *input, char *tokens[])
{
	char *token = strtok(input, " ");
	int tokenCount = 0;

	while (token != NULL && tokenCount < MAX_INPUT_SIZE) {
		tokens[tokenCount++] = token;
		token = strtok(NULL, " ");
	}

	return tokenCount;
}

enum Command parseCommand(char *cmd)
{
	if (strcmp(cmd, "exit") == 0) {
		return CMD_EXIT;
	} else if (strcmp(cmd, "cd") == 0) {
		return CMD_CD;
	} else if (strcmp(cmd, "cp") == 0) {
		return CMD_CP;
	} else if (strcmp(cmd, "mv") == 0) {
		return CMD_MV;
	} else if (strcmp(cmd, "rm") == 0) {
		return CMD_RM;
	} else if (strcmp(cmd, "rmdir") == 0) {
		return CMD_RMDIR;
	} else if (strcmp(cmd, "rename") == 0) {
		return CMD_RENAME;
	} else if (strcmp(cmd, "stat") == 0) {
		return CMD_STAT;
	} else if (strcmp(cmd, "gzip") == 0) {
		return CMD_GZIP;
	} else if (strcmp(cmd, "gunzip") == 0) {
		return CMD_GUNZIP;
	} else if (strcmp(cmd, "zip") == 0) {
		return CMD_ZIP;
	} else if (strcmp(cmd, "unzip") == 0) {
		return CMD_UNZIP;
	} else if (strcmp(cmd, "psinfo") == 0) {
		return CMD_PSINFO;
	} else if (strcmp(cmd, "sysinfo") == 0) {
		return CMD_SYSINFO;
	} else if (strcmp(cmd, "ascii") == 0) {
		return CMD_ASCII;
	} else if (strcmp(cmd, "search") == 0) {
		return CMD_SEARCH;
	} else if (strcmp(cmd, "jobs") == 0) {
		return CMD_JOBS;
	} else if (strcmp(cmd, "setcolor") == 0) {
		return CMD_SETCOLOR;
	}

	return CMD_OTHER;
}

void handleCommand(enum Command command, char *tokens[], int tokenCount)
{
	switch (command) {
		case CMD_EXIT:
			printf("Exiting Shell..\n");
			exit(0);
		case CMD_CD:
			if (tokenCount < 2) {
				if (changeDirectory(getenv("HOME")) != 0) {
					perror("Error: cd");
				}
			} else {
				if (changeDirectory(tokens[1]) != 0) {
					perror("Error: cd");
				}
			}
			break;
		case CMD_CP:
			if (tokenCount < 3) {
				printf("Usage: cp <source> <destination>\n");
			} else {
				if (copyFile(tokens[1], tokens[2]) != 0) {
					perror("Error: cp");
				}
			}
			break;
		case CMD_MV:
			if (tokenCount < 3) {
				printf("Usage: mv <source> <destination>\n");
			} else {
				if (moveFile(tokens[1], tokens[2]) != 0) {
					perror("Error: mv");
				}
			}
			break;
		case CMD_RM:
			if (tokenCount < 2) {
				printf("Usage: rm <file_name>\n");
			} else {
				if (deleteFile(tokens[1]) != 0) {
					perror("Error: rm");
				}
			}
			break;
		case CMD_RMDIR:
			if (tokenCount < 2) {
				printf("Usage: rmdir <directory_name>\n");
			} else {
				if (recursiveDelete(tokens[1]) != 0) {
					perror("Error: rmdir");
				}
			}
			break;
		case CMD_RENAME:
			if (tokenCount < 3) {
				printf("Usage: rename <old_name> <new_name>\n");
			} else {
				if (renameFile(tokens[1], tokens[2]) != 0) {
					perror("Error: mv");
				}
			}
			break;
		case CMD_STAT:
			if (tokenCount < 2) {
				printf("Usage: stat <file_or_directory>\n");
			} else {
				if (getFileInformation(tokens[1]) != 0) {
					perror("Error: stat");
				}
			}
			break;
		case CMD_GZIP:
			if (tokenCount < 2) {
				printf("Usage: gzip <file_name>\n");
			} else {
				if (compressFile(tokens[1]) != 0) {
					perror("Error: gzip");
				}
			}
			break;
		case CMD_GUNZIP:
			if (tokenCount < 2) {
				printf("Usage: gunzip <file_name.gz>\n");
			} else {
				if (decompressFile(tokens[1]) != 0) {
					perror("Error: gunzip");
				}
			}
			break;
		case CMD_ZIP:
			if (tokenCount < 3) {
				printf("Usage: zip <folder_path> <zip_file_name>\n");
			} else {
				if (compressFolder(tokens[1], tokens[2]) != 0) {
					perror("Error: zip");
				}
			}
			break;
		case CMD_UNZIP:
			if (tokenCount < 3) {
				printf("Usage: unzip <zip_file_path> <extract_path>\n");
			} else {
				if (decompressZip(tokens[1], tokens[2]) != 0) {
					perror("Error: unzip");
				}
			}
			break;
		case CMD_PSINFO:
			if (tokenCount != 2) {
				fprintf(stderr, "Usage: psinfo <PID>\n");
			} else {
				display_process_info(tokens[1]);
			}
			break;
		case CMD_SYSINFO:
			if (tokenCount != 2) {
				fprintf(stderr, "Usage: sysinfo <top N processes>\n");
			} else {
				display_system_info(tokens[1]);
			}
			break;
		case CMD_ASCII:
			if ((tokenCount != 2) || (strcmp(tokens[1], "enable") != 0 && strcmp(tokens[1], "disable") != 0)) {
				printf("Usage: ascii <enable OR disable>\n");
			} else {
				toggle_ascii_art(tokens[1]);
			}
			break;
		case CMD_SEARCH:
			if (tokenCount < 3) {
				printf("Usage: search <directory_path> <search_keyword>\n");
			} else {
				runFileSearch(tokens[1], tokens[2]);
			}
			break;
		case CMD_JOBS:
			if (tokenCount > 1) {
				printf("Usage: jobs\n");
			} else {
				print_jobs();
			}
			break;
		case CMD_SETCOLOR:
			if (tokenCount != 3) {
				printf("Usage: setcolor <username OR hostname OR currentdir> <COLOR>\n");
			} else {
				if (strcmp(tokens[1], "username") == 0) {
					setUsernameColor(tokens[2]);
				} else if (strcmp(tokens[1], "hostname") == 0) {
					setHostnameColor(tokens[2]);
				} else if (strcmp(tokens[1], "currentdir") == 0) {
					setCurrentDirColor(tokens[2]);
				} else {
					printf("Usage: setcolor <username OR hostname OR currentdir> <COLOR>\n");
				}
			}
			break;
		case CMD_OTHER:
			// check if the command should run in the background
			int run_in_background = (tokenCount > 1 && strcmp(tokens[tokenCount - 1], "&") == 0);

			// check for input/output redirection and piping
			int redirIndex = findRedirection(tokens, tokenCount);
			int pipeIndex = findPipe(tokens, tokenCount);

			if (redirIndex != -1) {
				handle_redirection(tokens, redirIndex, tokenCount);
			}

			if (pipeIndex != -1) {
				handle_pipe(tokens, pipeIndex, tokenCount);
			}

			// fork and execute command
			pid_t child_pid = fork();
			if (child_pid == -1) {
				perror("Error: Fork Failed");
				exit(1);
			}

			if (child_pid == 0) {
				// child process
				tokens[tokenCount] = NULL;  // Null-terminate the tokens array
				// handle other commands using execvp
				if (execvp(tokens[0], tokens) == -1) {
					perror("Error executing command");
					exit(1);
				}
			} else {
				// parent process
				if (!run_in_background) {
					// wait for the child to finish if it's not meant to run in the background
					foreground = child_pid;
					int status;
					waitpid(child_pid, &status, WUNTRACED);
					if (WIFSTOPPED(status)) {
						// child process was stopped by Ctrl+Z
						foreground = -1;
					}
				} else {
					// add job information to the linked list and print information
					add_job(child_pid, tokens[0]);
					printf("[%d] %d\n", get_next_job_id(), child_pid);
				}
			}
	}
}

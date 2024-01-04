#include <processinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysinfo.h>
#include <unistd.h>

#include "autocomplete.h"
#include "compression.h"
#include "config.h"
#include "directory.h"
#include "job_control.h"
#include "pipes.h"
#include "prompt.h"
#include "redirection.h"
#include "search.h"

#define MAX_INPUT_SIZE 1024
int main()
{
	// get saved shell config
	loadConfig();

	// setup job control signals
	setup_signal_handlers();

	char currentDir[PATH_MAX];
	char hostName[HOST_NAME_MAX];
	char username[LOGIN_NAME_MAX];

	initializeHistory();  // initialize command history
	rl_attempted_completion_function = (rl_completion_func_t *)autocomplete_generator;

	// display ascii
	displayASCII();

	while (1) {
		getPromptInfo(username, hostName, currentDir);
		char *prompt = malloc(MAX_INPUT_SIZE + 50);  // add extra space for the prompt
		sprintf(prompt, "%s%s@%s:%s%s$ %s", COLOR_LIGHT_PURPLE, username, hostName, COLOR_CYAN, currentDir, COLOR_RESET);

		char *input = readline(prompt);
		free(prompt);

		if (input == NULL) {
			break;
		}

		// if the input is not empty, add it to history
		if (strlen(input) > 0) {
			add_history(input);
		}

		char *tokens[MAX_INPUT_SIZE];

		// tokenize input
		char *token = strtok(input, " ");
		int tokenCount = 0;

		while (token != NULL) {
			// store the token in the tokens array
			tokens[tokenCount] = token;
			tokenCount++;

			token = strtok(NULL, " ");
		}

		// find the index of redirection and pipe symbols
		int redirIndex = findRedirection(tokens, tokenCount);
		int pipeIndex = findPipe(tokens, tokenCount);

		// handle input/output redirection
		if (redirIndex != -1) {
			handle_redirection(tokens, redirIndex, tokenCount);
		}

		// handle pipes
		if (pipeIndex != -1) {
			handle_pipe(tokens, pipeIndex, tokenCount);
		}

		// handle commands
		// quit shell on "exit" command
		if (tokenCount == 1 && strcmp(tokens[0], "exit") == 0) {
			printf("Exiting Shell..\n");
			free(input);
			break;
		}

		if (tokenCount > 0) {
			if (strcmp(tokens[0], "cd") == 0) {
				if (tokenCount < 2) {
					if (changeDirectory(getenv("HOME")) != 0) {
						perror("Error: cd");
					}
				} else {
					if (changeDirectory(tokens[1]) != 0) {
						perror("Error: cd");
					}
				}
			} else if (strcmp(tokens[0], "mkdir") == 0) {
				if (tokenCount < 2) {
					printf("Usage: mkdir <directory_name>\n");
				} else {
					if (createDirectory(tokens[1]) != 0) {
						perror("Error: mkdir");
					}
				}
			} else if (strcmp(tokens[0], "rmdir") == 0) {
				if (tokenCount < 2) {
					printf("Usage: rmdir <directory_name>\n");
				} else {
					if (deleteDirectory(tokens[1]) != 0) {
						perror("Error: rmdir");
					}
				}
			} else if (strcmp(tokens[0], "cp") == 0) {
				if (tokenCount < 3) {
					printf("Usage: cp <source> <destination>\n");
				} else {
					if (copyFile(tokens[1], tokens[2]) != 0) {
						perror("Error: cp");
					}
				}
			} else if (strcmp(tokens[0], "mv") == 0) {
				if (tokenCount < 3) {
					printf("Usage: mv <source> <destination>\n");
				} else {
					if (moveFile(tokens[1], tokens[2]) != 0) {
						perror("Error: mv");
					}
				}
			} else if (strcmp(tokens[0], "rm") == 0) {
				if (tokenCount < 2) {
					printf("Usage: rm <file_name>\n");
				} else {
					if (recursiveDelete(tokens[1]) != 0) {
						perror("Error: rm");
					}
				}
			} else if (strcmp(tokens[0], "rename") == 0) {
				if (tokenCount < 3) {
					printf("Usage: rename <old_name> <new_name>\n");
				} else {
					if (renameFile(tokens[1], tokens[2]) != 0) {
						perror("Error: mv");
					}
				}
			} else if (strcmp(tokens[0], "stat") == 0) {
				if (tokenCount < 2) {
					printf("Usage: stat <file_or_directory>\n");
				} else {
					if (getFileInformation(tokens[1]) != 0) {
						perror("Error: stat");
					}
				}
			} else if (strcmp(tokens[0], "gzip") == 0) {
				if (tokenCount < 2) {
					printf("Usage: gzip <file_name>\n");
				} else {
					if (compressFile(tokens[1]) != 0) {
						perror("Error: gzip");
					}
				}
			} else if (strcmp(tokens[0], "gunzip") == 0) {
				if (tokenCount < 2) {
					printf("Usage: gunzip <file_name.gz>\n");
				} else {
					if (decompressFile(tokens[1]) != 0) {
						perror("Error: gunzip");
					}
				}
			} else if (strcmp(tokens[0], "zip") == 0) {
				if (tokenCount < 3) {
					printf("Usage: zip <folder_path> <zip_file_name>\n");
				} else {
					if (compressFolder(tokens[1], tokens[2]) != 0) {
						perror("Error: zip");
					}
				}
			} else if (strcmp(tokens[0], "unzip") == 0) {
				if (tokenCount < 3) {
					printf("Usage: unzip <zip_file_path> <extract_path>\n");
				} else {
					if (decompressZip(tokens[1], tokens[2]) != 0) {
						perror("Error: unzip");
					}
				}
			} else if (strcmp(tokens[0], "psinfo") == 0) {
				if (tokenCount != 2) {
					fprintf(stderr, "Usage: psinfo <PID>\n");
				} else {
					display_process_info(tokens[1]);
				}
				continue;  // skip the rest of the shell logic for this command
			} else if (strcmp(tokens[0], "sysinfo") == 0) {
				if (tokenCount != 2) {
					fprintf(stderr, "Usage: sysinfo <top N processes>\n");
				} else {
					display_system_info(tokens[1]);
				}
			

			
			} else if ((strcmp(tokens[0], "ascii") == 0)) {
				if ((tokenCount != 2) || (strcmp(tokens[1], "enable") != 0 && strcmp(tokens[1], "disable") != 0)) {
					printf("Usage: ascii <enable OR disable>\n");
				} else {
					toggle_ascii_art(tokens[1]);
				}
			} else if (strcmp(tokens[0], "search") == 0) {
				if (tokenCount < 3) {
					printf("Usage: search <directory_path> <search_keyword>\n");
				} else {
					runFileSearch(tokens[1], tokens[2]);
				}

			} else {				
				// handle other commands using execvp
				// create a child process
				
				pid_t child_pid = fork();

				if (child_pid == -1) {
					perror("Error: Fork Failed");
					free(input);
					exit(1);
				}

				if (child_pid == 0) {
					// child process

					// null-terminate the tokens array
					tokens[tokenCount] = NULL;

					if (execvp(tokens[0], tokens) == -1) {
						perror("Error executing command");
						free(input);
						exit(1);
					}
				} else {
					// parent process
					foreground = child_pid;	 // set the current foreground process PID

					// wait for the child to finish
					int status;
					waitpid(child_pid, &status, WUNTRACED);
					if (WIFSTOPPED(status)) {
						// the child process was stopped by Ctrl+Z
						foreground = -1;  // reset current foreground process PID
					}
				}
			}
		}

		free(input);
	}

	return 0;
}

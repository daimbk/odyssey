#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_INPUT_SIZE 1024

// define enums for commands
enum Command {
	CMD_EXIT,
	CMD_CD,
	CMD_CP,
	CMD_MV,
	CMD_RM,
	CMD_RMDIR,
	CMD_RENAME,
	CMD_STAT,
	CMD_GZIP,
	CMD_GUNZIP,
	CMD_ZIP,
	CMD_UNZIP,
	CMD_PSINFO,
	CMD_SYSINFO,
	CMD_ASCII,
	CMD_HEHE,
	CMD_SEARCH,
	CMD_JOBS,
	CMD_SETCOLOR,
	CMD_OTHER
};

int tokenizeInput(char *input, char *tokens[]);
enum Command parseCommand(char *cmd);
void handleCommand(enum Command command, char *tokens[], int tokenCount);

#endif

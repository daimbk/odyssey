#ifndef REDIRECTION_H
#define REDIRECTION_H

void handle_redirection(char **tokens, int redirIndex, int tokenCount);
int findRedirection(char **tokens, int tokenCount);

#endif

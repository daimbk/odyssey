#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

char *get_completion(int index);
char *autocomplete_generator(const char *text, int state);

#endif

#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>


#define MAX_LENGTH 1024

#define UNUSED(x) (void)(x)

char *get_shell_name();
void handle_cd(char *args[], const char *shell_name, int command_count);
int tokenize_input(char *input, char *args[]);
int fork_strtok(char *input);
int _print(const char *str);
size_t _strlen(const char *str);

#endif



#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>

#define MAX_LENGTH 1024

#define UNUSED(x) (void)(x)

extern char **environ;

int execute_command(char *args[], const char *shell_name, int command_count);
int search_n_exec_cmd(char *args[], const char *shell_name, int command_count);
int handle_env(const char *shell_name, int command_count);
int exiT(char *args[], const char *shell_name, int command_count,
int status, char *input);
int chK(char *args[], const char *shell_name, int command_count, int status,
char *input);
char *get_shell_name();
void handle_cd(char *args[], const char *shell_name, int command_count);
int tokenize_input(char *input, char *args[]);
int fork_strtok(char *input);
int _print(const char *str);
size_t _strlen(const char *str);
int check_for_non_digit(const char *str);

#endif

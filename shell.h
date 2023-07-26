#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_LENGTH 1024


extern char **environ;

void process_input(char *input, const char *shell_name, int *command_count_ptr);
int execute_command(char *args[], const char *shell_name, int command_count);
int tokenize_input(char *input, char *args[]);
void handle_cd(char *args[], const char *shell_name, int command_count);
void search_n_exec_cmd(char *args[], const char *shell_name, int command_count);
char *get_shell_name();
void remove_all_env_except_one(const char *env_var_to_keep);
void cleanup(char *input);
void process_exit_command(char *input);
void shell_loop(const char *shell_name);
int fork_strtok(char *input);
int _print(const char *str);
size_t _strlen(const char *str);

#endif



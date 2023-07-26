#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH 1024

// Function declarations
int execute_command(char *args[], int command_count);
int tokenize_input(char *input, char *args[]);
void handle_cd(char *args[], int command_count);
int search_n_exec_cmd(char *args[], int command_count);
char *get_shell_name();
void remove_all_env_except_one(const char *env_var_to_keep);
void cleanup(char *input);
void process_input(char *input, int *command_count_ptr);

/*
 * execute_command - Function to execute a command.
 * @args: Array of arguments for the command.
 * @command_count: The count of commands entered since shell execution.
 *
 * Return: (0) on success, (-1) on failure.
 */
int execute_command(char *args[], int command_count)
{
    char error_msg[MAX_LENGTH];
    pid_t pid = fork();
    if (pid < 0)
    {
        snprintf(error_msg, MAX_LENGTH, "Shell: %d: Fork failed\n", command_count);
        fprintf(stderr, "%s", error_msg);
        return (-1);
    }
    else if (pid == 0)
    {
        execvp(args[0], args);
        snprintf(error_msg, MAX_LENGTH, "Shell: %d: %s: not found", command_count, args[0]);
        fprintf(stderr, "%s\n", error_msg);
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
    return (0);
}

/*
 * tokenize_input - Function to tokenize user input.
 * @input: User input string.
 * @args: Array to store the tokenized arguments.
 *
 * Return: Number of arguments tokenized.
 */
int tokenize_input(char *input, char *args[])
{
    int i = 0;
    char *token = strtok(input, " \t\n\r");

    while (token != NULL && i < MAX_LENGTH - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n\r");
    }
    args[i] = NULL;

    return i;
}

/*
 * handle_cd - Function to handle built-in "cd" command.
 * @args: Array of arguments for the command.
 * @command_count: The count of commands entered since shell execution.
 */
void handle_cd(char *args[], int command_count)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Shell: %d: Usage: cd <directory>\n", command_count);
    }
    else if (chdir(args[1]) != 0)
    {
        char error_msg[MAX_LENGTH];
        snprintf(error_msg, MAX_LENGTH, "cd: can't cd to %s", args[1]);
        fprintf(stderr, "%s\n", error_msg);
    }
}

/*
 * search_n_exec_cmd - Function to search and execute command in PATH.
 * @args: Array of arguments for the command.
 * @command_count: The count of commands entered since shell execution.
 *
 * Return: (0) on success, (-1) if command not found.
 */
int search_n_exec_cmd(char *args[], int command_count)
{
    char path[MAX_LENGTH], full_path[MAX_LENGTH];
    char *path_token, *path_env;
    int found = 0;

    path_env = getenv("PATH");
    if (path_env == NULL || strlen(path_env) == 0)
    {
        if (access(args[0], X_OK) == 0)
        {
            found = 1;
        }
    }
    else
    {
        strcpy(path, path_env);
        path_token = strtok(path, ":");

        while (path_token != NULL)
        {
            strcpy(full_path, path_token);
            strcat(full_path, "/");
            strcat(full_path, args[0]);
            if (access(full_path, X_OK) == 0)
            {
                found = 1;
                break;
            }
            path_token = strtok(NULL, ":");
        }
    }

    if (found)
    {
        execute_command(args, command_count);
        free(args[0]);
        return (0);
    }
    else
    {
        return (-1);
    }
}

/*
 * get_shell_name - Function to get the name of the shell.
 *
 * Return: The name of the shell.
 */
char *get_shell_name()
{
    char *shell_name;
    shell_name = getenv("_");
    return shell_name;
}

/*
 * remove_all_env_except_one - Function to remove all environment variables except one.
 * @env_var_to_keep: The environment variable to keep.
 */
void remove_all_env_except_one(const char *env_var_to_keep)
{
    char **env_copy = NULL;
    int count = 0;
    char **env;
    int index = 0;

    for (env = environ; *env != NULL; ++env)
    {
        if (strncmp(env_var_to_keep, *env, strlen(env_var_to_keep)) == 0)
        {
            count++;
        }
    }

    env_copy = (char **)malloc((count + 1) * sizeof(char *));
    if (env_copy == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (env = environ; *env != NULL; ++env)
    {
        if (strncmp(env_var_to_keep, *env, strlen(env_var_to_keep)) == 0)
        {
            env_copy[index++] = strdup(*env);
        }
    }
    env_copy[index] = NULL;

    clearenv();
    for (index = 0; env_copy[index] != NULL; index++)
    {
        if (putenv(env_copy[index]) != 0)
        {
            fprintf(stderr, "Failed to set environment variable\n");
            exit(EXIT_FAILURE);
        }
    }

    for (index = 0; env_copy[index] != NULL; index++)
    {
        free(env_copy[index]);
    }
    free(env_copy);
}

/*
 * cleanup - Function to clean up input.
 * @input: The user input string.
 */
void cleanup(char *input)
{
    if (isatty(STDIN_FILENO) == 1)
        printf("$ ");
    free(input);
    exit(0);
}

/*
 * process_input - Function to process user input.
 * @input: The user input string.
 * @command_count_ptr: Pointer to the count of commands entered since shell execution.
 */
void process_input(char *input, int *command_count_ptr)
{
    ssize_t read_size;
    size_t input_len = 0;

    if (isatty(STDIN_FILENO) == 1)
        printf("$ ");

    read_size = getline(&input, &input_len, stdin);
    if (read_size == -1)
    {
        if (isatty(STDIN_FILENO) == 1)
            printf("\n");
        cleanup(input);
    }
    (*command_count_ptr)++;
}

int execute_cmd_with_path(char *args[], int command_count)
{
    int result = search_n_exec_cmd(args, command_count);
    free(args[0]);
    return (result);
}

void shell_loop()
{
    char *input = NULL, *args[MAX_LENGTH];
    int command_count = 0, num_args;

    remove_all_env_except_one("PATH");
    system("env");

    while (1)
    {
        process_input(input, &command_count);

        if (strcmp(input, "exit") == 0)
        {
            num_args = tokenize_input(input, args);
            if (num_args == 1)
            {
                cleanup(input);
            }
            if (num_args == 2)
            {
                int exit_status = atoi(args[1]);
                cleanup(input);
                exit(exit_status);
            }
        }

        num_args = tokenize_input(input, args);
        if (num_args == 0)
        {
            cleanup(input);
        }

        if (strcmp(args[0], "cd") == 0)
        {
            handle_cd(args, command_count);
            cleanup(input);
        }

        if (strcmp(args[0], "clearenv") == 0)
        {
            clearenv();
            system("env");
            cleanup(input);
        }

        if (strchr(args[0], '/') == NULL)
        {
            if (execute_cmd_with_path(args, command_count) == -1)
            {
                fprintf(stderr, "Shell: %d: %s: command not found\n", command_count, args[0]);
            }
        }
        else
        {
            execute_command(args, command_count);
        }

        cleanup(input);
    }
}

int main(void)
{
    shell_loop();
    return (0);
}


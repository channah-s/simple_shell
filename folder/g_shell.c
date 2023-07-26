#include "shell.h"

/**
 * execute_command - Function to execute a command
 * @args: Array of arguments for the command
 * @shell_name: The name of the shell (e.g., "sh")
 * @command_count: The count of commands entered since shell execution
 *
 * Return: 0 on success, -1 on failure
 */
int execute_command(char *args[], const char *shell_name, int command_count)
{
	char error_msg[MAX_LENGTH];

	pid_t pid = fork();

	if (pid < 0)
	{
		fprintf(stderr, "%s: %d: Fork failed\n", shell_name, command_count);
		return (-1);
	}
	else if (pid == 0)
	{
		execvp(args[0], args);
		snprintf(error_msg, MAX_LENGTH, "%s: %d: %s: not found",
		shell_name, command_count, args[0]);
		fprintf(stderr, "%s\n", error_msg);
		exit(EXIT_FAILURE);
	}
	else
		wait(NULL);
	return (0);
}

/**
 * search_n_exec_cmd - Function to search and execute command in PATH
 * @args: Array of arguments for the command
 * @shell_name: The name of the shell (e.g., "sh")
 * @command_count: The count of commands entered since shell execution
 */
void search_n_exec_cmd(char *args[], const char *shell_name, int command_count)
{
	char path[MAX_LENGTH], full_path[MAX_LENGTH];
	char *path_token, *path_env;
	int found = 0;

	path_env = getenv("PATH");
	if (path_env == NULL || strlen(path_env) == 0)
	{
		if (access(args[0], X_OK) == 0)
			found = 1;
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
				break; /* Command found, no need to continue searching */
			}
			path_token = strtok(NULL, ":");
		}
	}

	if (found)
		execute_command(args, shell_name, command_count);
	else
	{
		char error_msg[MAX_LENGTH];

		snprintf(error_msg, MAX_LENGTH, "%s: %d: %s: not found",
		shell_name, command_count, args[0]);
		fprintf(stderr, "%s\n", error_msg);
	}
}

/**
 * shell_loop - Main shell loop
 * @shell_name: The name of the shell (e.g., "sh")
 */
void shell_loop(const char *shell_name)
{
	char *input = NULL, *args[MAX_LENGTH];
	int command_count = 0, num_args;

	remove_all_env_except_one("PATH");
	while (1)
	{
		command_count++;
		cleanup(&input);
		if (strcmp(input, "exit") == 0)
		{
			num_args = tokenize_input(input, args);
			if (num_args == 1)
			{
				free(input);
				exit(0);
			}
			if (num_args == 2)
			{
				int exit_status = atoi(args[1]);

				free(input);
				exit(exit_status);
			}
		}
		num_args = tokenize_input(input, args);

		if (num_args == 0)
			continue;
		if (strcmp(args[0], "cd") == 0)
		{
			handle_cd(args, shell_name, command_count);
			continue;
		}
		if (strchr(args[0], '/') == NULL)
			search_n_exec_cmd(args, shell_name, command_count);
		else
			execute_command(args, shell_name, command_count);
	}
	free(input);
}

/**
 * main - Main Function
 *
 * Return: Always 0
 */
int main(void)
{
	const char *shell_name = get_shell_name();

	shell_loop(shell_name);
	return (0);
}



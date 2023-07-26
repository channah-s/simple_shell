#include "shell.h"

/**
 * execute_command - Function to execute a command.
 *
 * @args: Array of arguments for the command.
 * @shell_name: The name of the shell (e.g., "sh").
 * @command_count: The count of commands entered since shell execution.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute_command(char *args[], const char *shell_name, int command_count)
{
	pid_t pid;
	char error_msg[MAX_LENGTH];

	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "%s: %d: Fork failed\n",
		shell_name, command_count);
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
 * search_n_exec_cmd - Function to search and execute command in PATH.
 *
 * @args: Array of arguments for the command.
 * @shell_name: The name of the shell (e.g., "sh").
 * @command_count: The count of commands entered since shell execution.
 */
void search_n_exec_cmd(char *args[], const char *shell_name, int command_count)
{
	char path[MAX_LENGTH], full_path[MAX_LENGTH];
	char *path_token, *path_env;
	int found = 0;
	char error_msg[MAX_LENGTH];

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
				break;
			}
			path_token = strtok(NULL, ":");
		}
	}

	if (found)
		execute_command(args, shell_name, command_count);
	else
	{
		snprintf(error_msg, MAX_LENGTH, "%s: %d: %s: not found",
		shell_name, command_count, args[0]);
		fprintf(stderr, "%s\n", error_msg);
	}
}

/**
 * get_shell_name - Function to get the name of the shell.
 *
 * Return: The name of the shell.
 */
char *get_shell_name()
{
	char *shell_name;

	shell_name = getenv("_");
	return (shell_name);
}

/**
 * remove_all_env_except_one - remove all environment variables except 1.
 *
 * @env_var_to_keep: The environment variable to keep.
 */
void remove_all_env_except_one(const char *env_var_to_keep)
{
	char **env;
	char **env_copy = NULL;
	int count = 0, index = 0;

	for (env = environ; *env != NULL; ++env)
	{
		if (strncmp(env_var_to_keep, *env, strlen(env_var_to_keep)) == 0)
			count++;
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
			env_copy[index++] = strdup(*env);
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
		free(env_copy[index]);
	free(env_copy);
}

/**
 * main - Main Function.
 */
int main(void)
{
	const char *shell_name = get_shell_name();

	shell_loop(shell_name);
	return (0);
}

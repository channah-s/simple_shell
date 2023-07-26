#include "shell.h"

/**
 * process_exit_command - Function to process the "exit" command.
 *
 * @input: User input string.
 */
void process_exit_command(char *input)
{
	char *args[MAX_LENGTH];
	int num_args, exit_status;

	num_args = tokenize_input(input, args);

	if (num_args == 1)
	{
		free(input);
		exit(0);
	}
	else if (num_args == 2)
	{
		exit_status = atoi(args[1]);
		free(input);
		exit(exit_status);
	}
}

/**
 * process_input - Function to process user input.
 *
 * @input: User input string.
 * @shell_name: The name of the shell (e.g., "sh").
 * @command_count_ptr: pointer to the command pointer
 */
void process_input(char *input, const char *shell_name, int *command_count_ptr)
{
	char *args[MAX_LENGTH];
	int num_args = tokenize_input(input, args);

	if (num_args == 0)
		return;

	if (strcmp(args[0], "cd") == 0)
	{
		handle_cd(args, shell_name, *command_count_ptr);
		return;
	}

	if (strcmp(args[0], "clearenv") == 0)
	{
		clearenv();
		system("env");
		return;
	}

	if (strchr(args[0], '/') == NULL)
		search_n_exec_cmd(args, shell_name, *command_count_ptr);
	else
		execute_command(args, shell_name, *command_count_ptr);
}


/**
 * shell_loop - Main shell loop.
 *
 * @shell_name: The name of the shell.
 */
void shell_loop(const char *shell_name)
{
	char *input = NULL, *args[MAX_LENGTH];
	int command_count = 0, exit_status, num_args;
	size_t size = 0;

	remove_all_env_except_one("PATH");
	system("env");
	while (1)
	{
		command_count++;
		cleanup(input);
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
				exit_status = atoi(args[1]);
				free(input);
				exit(exit_status);
			}
		}
		process_input(input, shell_name, &command_count);

		if (isatty(STDIN_FILENO) == 1)
			printf("$ ");
		if (getline(&input, &size, stdin) == -1)
			if (isatty(STDIN_FILENO) == 1)
				printf("\n");
	}
	free(input);
}

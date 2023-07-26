#include "shell.h"

/**
 * cleanup - Function to clean up input.
 *
 * @input: The user input string.
 */
void cleanup(char *input)
{
	size_t size = 0;

	if (isatty(STDIN_FILENO) == 1)
		printf("$ ");

	if (getline(&input, &size, stdin) == -1)
		if (isatty(STDIN_FILENO) == 1)
			printf("\n");
	free(input);
	exit(0);
}

/**
 * tokenize_input - Function to tokenize user input.
 *
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

	return (i);
}

/**
 * handle_cd - Function to handle built-in "cd" command.
 *
 * @args: Array of arguments for the command.
 * @shell_name: The name of the shell (e.g., "sh").
 * @command_count: The count of commands entered since shell execution.
 */
void handle_cd(char *args[], const char *shell_name, int command_count)
{
	char error_msg[MAX_LENGTH];

	if (args[1] == NULL)
	{
		fprintf(stderr, "%s: %d: Usage: cd <directory>\n",
				shell_name, command_count);
		return;
	}

	if (chdir(args[1]) != 0)
	{
		snprintf(error_msg, MAX_LENGTH, "cd: can't cd to %s", args[1]);
		fprintf(stderr, "%s\n", error_msg);
	}
}



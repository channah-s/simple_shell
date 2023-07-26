#include "shell.h"

/**
 * tokenize_input - Function to tokenize user input
 * @input: User input string
 * @args: Array to store the tokenized arguments
 *
 * Return: Number of arguments tokenized
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
 * handle_cd - Function to handle built-in "cd" command
 * @args: Array of arguments for the command
 * @shell_name: The name of the shell (e.g., "sh")
 * @command_count: The count of commands entered since shell execution
 */
void handle_cd(char *args[], const char *shell_name, int command_count)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "%s: %d: Usage: cd <directory>\n",
				shell_name, command_count);
	}
	else if (chdir(args[1]) != 0)
	{
		char error_msg[MAX_LENGTH];

		snprintf(error_msg, MAX_LENGTH, "cd: can't cd to %s", args[1]);
		fprintf(stderr, "%s\n", error_msg);
	}
}
/**
 * get_shell_name - Function to get the name of the shell
 *
 * Return: The name of the shell
 */
char *get_shell_name()
{
	char *shell_name;

	shell_name = getenv("_");
	return (shell_name);
}

/**
 * remove_all_env_except_one - remove all environment variables except 1
 * @env_var_to_keep: The environment variable to keep
 */
void remove_all_env_except_one(const char *env_var_to_keep)
{
	size_t i;

	for (i = 0; environ[i] != NULL; ++i)
	{
		if (strncmp(env_var_to_keep, environ[i], strlen(env_var_to_keep)) != 0)
			unsetenv(environ[i]);
	}
}

/**
 * cleanup - Function to clean up input
 * @input: Double pointer to the input string
 */
void cleanup(char **input)
{
	size_t n = 0;

	if (isatty(STDIN_FILENO) == 1)
		printf("$ ");

	if (getline(input, &n, stdin) == -1)
	{
		if (isatty(STDIN_FILENO) == 1)
			printf("\n");
		free(*input);
		exit(0);
	}

	(*input)[strcspn(*input, "\n")] = '\0';
}

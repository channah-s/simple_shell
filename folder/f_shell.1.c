#include "shell.h"

/**
 * tokenize_input - function to tokenize user input
 * @input: user input string
 * @args: array to store the tokenized arguments
 * Return: number of arguments tokenized
 */
int tokenize_input(char *input, char *args[])
{
	int i = 0;
	char *token = strtok(input, " \t\n\r");

	while (token != NULL)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t\n\r");
	}
	args[i] = NULL;

	return (i);
}

/**
 * handle_cd - function to handle built-in "cd" command
 * @args: array of arguments for the command
 * @shell_name: the name of the shell (e.g., "sh")
 * @command_count: the count of commands entered since shell execution
 */
void handle_cd(char *args[], const char *shell_name, int command_count)
{
	if (args[1] == NULL)
		fprintf(stderr, "%s: %d: Usage: cd <directory>\n",
				shell_name, command_count);
	if (chdir(args[1]) != 0)
		fprintf(stderr, "%s: %d: cd: can't cd to %s\n",
				shell_name, command_count, args[1]);
}


/**
 * get_shell_name - function to get the name of the shell
 * Return: the name of the shell
 */

char *get_shell_name()
{
	char *shell_name;

	shell_name = getenv("_");
	return (shell_name);
}

#include "shell.h"

/**
 * check_for_non_digit - checks if string contains non-digits
 * @str: string to check its content for non-digits
 * Return: true if there is a non-digit, else false if all digits
 */

int check_for_non_digit(const char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (1);
		str++;
	}

	return (0);

}

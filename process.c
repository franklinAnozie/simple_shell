#include "main.h"

static char **words;
static int wordCount;

/**
 * exe_bin - function that execute a binary program
 * @args: arguments to be passed to the program
 * Return: 0 on sucess, -1 on failure
*/
int exe_bin(char **args)
{
	if (access(args[0], X_OK | F_OK) == 0)
	{
		pid_t child = fork();

		if (child == -1)
			perror("./hsh");
		if (child != 0)
		{
			wait(NULL);
		} else
		{
			int val = execve(args[0], args, environ);

			if (val == -1)
				perror("./hsh");
			return (-1);
		}
	}

	return (0);
}

/**
 * exe_cmd -  function that execute command
 * @args: array of arguments for the command
 * Return: 0 on sucess, -1 on failure
*/
int exe_cmd(char **args)
{
	int rtVal = 0;

	if (strcmp(args[0], "unsetenv") == 0)
	{
		unsetenv(args[1]);
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		chgdir(args[1]);
	}
	else if (strcmp(args[0], "exit") == 0)
	{
		exit_status(args[1]);
	}
	else if (strcmp(args[0], "setenv") == 0)
	{
		_setenv(args[1], args[2]);
	} else
	{
		args[0] = get_path(args[0]);
		rtVal = exe_bin(args);
	}
	return (rtVal);
}

/**
 * exit_status - Exit function with exit status
 * @input: exit status code/number
*/
void exit_status(const char *input)
{
	if (input != NULL)
	{
		int status = _atoi(input);

		freeWords(words, wordCount);
		exit(status);
	} else
	{
		freeWords(words, wordCount);
		exit(EXIT_SUCCESS);
	}
}

/**
 * prompt -  function that prompts the user for input
 * Return: 0 on success and -1 on failure
*/
int prompt(void)
{
	char *line = NULL;
	size_t line_size;
	ssize_t status_var;
	char *delim = " \n";
	int rtVal = -1, i = 0;

	if (isatty(STDIN_FILENO))
	{
		output("($) ");
		/* printf("Input is from terminal.\n"); */
	}
	status_var = getline(&line, &line_size, stdin);
	while (line[i] != '\n')
		i++;

	line[i] = '\0';


	/* Test that input was successful and exit if not */
	if (status_var > 0)
	{
		/* invoke the word separator function and get number of words read */
		wordCount = seperate_word(line, &words, i, delim);
		free(line);
	}
	rtVal = exe_cmd(words);
	freeWords(words, wordCount);
	return (rtVal);
}

/**
 * seperate_word - Function that seperates the words
 * in a string into a NULL terminated array of words
 * @line: The line of string to be operated
 * @words: Array to store the words
 * @line_size: size of the line string
 * @delim: Delimiter to be used for tokenization
 * Return: On success number of words tokenized, -1 on failure
*/
int seperate_word(char *line,
char ***words, int line_size, char *delim)
{
	int i;
	int wordCount = -1;
	char *token = NULL;


	if (line != NULL && _strcmp(line, "") != 0 && _strcmp(line, " ") != 0)
	{
		wordCount = 1;
		for (i = 0; i < line_size; i++)
		{
			if (line[i] == *delim)
			{
				wordCount++;
			}
		}

		*words = (char **)malloc(sizeof(char *) * (wordCount + 1));
		if (*words != NULL)
		{
			token = strtok(line, delim);

			i = 0;
			while (token != NULL)
			{
				(*words)[i] = _strdup(token);
				token = strtok(NULL, delim);
				i++;
			}
			(*words)[i] = NULL;
		}
		/* free(line); */
	}
	return (wordCount);
}

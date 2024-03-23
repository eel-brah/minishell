#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libft/include/libft.h"
#include <stdbool.h>

void	args_to_nargs(char **ntokens, char **tokens, char *arg)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		ntokens[i] = tokens[i];
		i++;
	}
	ntokens[i] = arg;
	ntokens[i + 1] = NULL;
}

char	**first_realloc(char *arg, size_t size)
{
	char	**ntokens;

	ntokens = malloc(sizeof(char *) * size);
	if (!ntokens)
		return (NULL);
	ntokens[0] = arg;
	ntokens[1] = NULL;
	return (ntokens);
}

char	**tokens_realloc(char **tokens, char *arg)
{
	size_t	size;
	char	**ntokens;

	size = 2;
	if (!tokens)
		return (first_realloc(arg, size));
	else
	{
		while (tokens[size - 1])
			size++;
		ntokens = malloc(sizeof(char *) * (size + 1));
		if (!ntokens)
			return (NULL);
		args_to_nargs(ntokens, tokens, arg);
	}
	free(tokens);
	return (ntokens);
}

// void	free_tokens(char **tokens)
// {
// 	size_t	i;

// 	i = 0;
// 	while (tokens && tokens[i])
// 		free(tokens[i++]);
// 	free(tokens);
// }

char	**add_token(char **tokens, char *cmd)
{
	char	*token;
	char	**tmp;

	// token = ft_strdup(cmd);
	// if (!token)
	// {
	// 	free_tokens(tokens);
	// 	return (NULL);
	// }
	token = cmd;
	tmp = tokens_realloc(tokens, token);
	if (!tmp)
	{
		// free_tokens(tokens);
		free(tokens);
		return (NULL);
	}
	return (tmp);
}

int    is_exist(int c, char *s)
{
    int    i;

    i = 0;
    while (s[i])
        if (s[i++] == c)
            return (1);
    return (0);
}
char    *ft_strchr2(const char *s, char *c)
{
    int        i;

    i = 0;
	if (!s || !c)
		return (NULL);
    while (s[i])
    {
        if (is_exist(s[i], c))
            return ((char *)&s[i]);
        i++;
    }
    return (NULL);
}

char	*quoets_handler(char *cmd)
{
	bool	quoets;
	char	c;
	char	*tmp;
	char	*ptr;
	char	*ptr2;

	c = *cmd;
	quoets = false;
	tmp = cmd + 1;
	while (1)
	{
		ptr = ft_strchr(tmp, c);
		if (!ptr)
		{
			if (!*tmp || !quoets)
			{
				write (2, "syntax error\n", 14);
				return (NULL);
			}
			return (&tmp[ft_strlen(tmp)]);
		}
		else if (ptr)
		{
			quoets = !quoets;
			if (quoets)
			{
				ptr2 = ft_strchr2(ptr + 1, "'\"\t\n\v\f\r ");
				if (!ptr2)
					return (&ptr[ft_strlen(ptr)]);
				else if (is_exist(*ptr2, "\t\n\v\f\r "))
					return (ptr2);
				else if (is_exist(*ptr2, "'\"") && *ptr2 != c)
					c = *ptr2;
			}
			tmp = ptr + 1;
		}
	}
	return (NULL);
}

char	**split_command(char *cmd)
{
	char	*ptr;
	char	**tokens;
	bool	end;

	tokens = NULL;
	end = false;
	while (*cmd)
	{
		ptr = ft_strchr2(cmd, "\t\n\v\f\r '\"");
		if (ptr && *ptr)
		{
			if (*ptr == '"' || *ptr == '\'')
				ptr = quoets_handler(cmd);
			if (!ptr)
				return (NULL);
			if (!*ptr)
				end = true;
			*ptr = 0;
			tokens = add_token(tokens, cmd);
			if (!tokens)
				return (NULL);
			if (end)
				break ;
			cmd = ptr + 1;
			while (is_exist(*cmd, "\t\n\v\f\r "))
				cmd++;
		}
		else
		{
			tokens = add_token(tokens, cmd);
			if (!tokens)
				return (NULL);
			break ;
		}
	}
	return (tokens);
}

void fu()
{
	system("leaks a.out");
}

int main()
{
	atexit(fu);
	char	*line;
	char	**tokens = NULL;
	char	*cmd;
	char	**ptr;
	int i = 3;
	while (1)
	{
		line = readline("$ ");
		if (line && *line) 
		{
			add_history (line);
			cmd = ft_strtrim(line, "\t\n\v\f\r ");
			if (!cmd)
				perror("malloc");
			else
			{
				tokens = split_command(cmd);
				ptr = tokens;
				while (tokens && *tokens)
				{
					printf("%s\n", *tokens);
					tokens++;
				}
				// free_tokens(tokens);
				free(ptr);
				free(cmd);
			}
		}
		// free(line);
	}
	// rl_clear_history();

}
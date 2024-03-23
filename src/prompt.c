#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../libft/include/libft.h"

void interpret_escape_sequences(char* str)
{
    char	*pos;

    while ((pos = ft_strnstr(str, "\\033", ft_strlen(str))) != NULL)
	{
        ft_memmove(pos + 1, pos + 4, strlen(pos + 4) + 1);
        *pos = '\033';
    }
}

char	*get_evnr(char **user, char **name, char **cwd)
{
	// char	*prompt;
	char	**ptr;
	char	**t_ptr;

	// prompt = getenv("PRPT");
	// if (prompt)
	// {
	// 	interpret_escape_sequences(prompt);
	// 	return (prompt);
	// }
	*user = getenv("USER");
	*name = getenv("NAME");
	*cwd = getenv("PWD");
	if (*cwd && ft_strlen(*cwd) > 1)
	{
		ptr = ft_split(*cwd, '/');
		if (ptr)
		{
			t_ptr = ptr;
			while (*ptr)
				*cwd = *ptr++;
			*cwd = ft_strdup(*cwd);
			ptr = t_ptr;
			while (*ptr)
				free(*ptr++);
			free(t_ptr);
		}
	}
	return (NULL);
}

char	*get_prompt()
{
	char	*user;
	char	*name;
	char	*cwd;
	char	*prompt;
	char	*tmp[9];
	int		i;
	int		j;
	
	prompt = get_evnr(&user, &name, &cwd);
	if (prompt)
		return (prompt);
	i = 0;
	if (user)
	{
		// prompt = ft_strjoin(prompt, "\033[01;32m");
		// tmp[i++] = prompt;
		prompt = ft_strjoin(prompt, user);
		tmp[i++] = prompt;
		if (name)
		{
			prompt = ft_strjoin(prompt, "@");
			tmp[i++] = prompt;
			prompt = ft_strjoin(prompt, name);
			tmp[i++] = prompt;
		}
		// prompt = ft_strjoin(prompt, "\033[0m");
		// tmp[i++] = prompt;
		if (cwd)
		{
			prompt = ft_strjoin(prompt, " ");
			tmp[i++] = prompt;
			// prompt = ft_strjoin(prompt, "\033[01;34m");
			// tmp[i++] = prompt;
			prompt = ft_strjoin(prompt, cwd);
			tmp[i++] = prompt;
			// prompt = ft_strjoin(prompt, "\033[0m");
			// tmp[i++] = prompt;
			if (ft_strlen(cwd) > 1)
				free (cwd);
		}
	}
	prompt = ft_strjoin(prompt, "$ ");
	j = 0;
	while (j < i)
		free(tmp[j++]);
	return (prompt);
}

// This function gives the user the default behaviour of TAB completion: 
// completion on file names. If you do not want Readline to complete on filenames, 
// you can change the binding of the TAB key with rl_bind_key ().
// int rl_bind_key (int key, int (*function)());
// int main()
// {
// 	char	*line;
// 	char	*prompt = get_prompt();

// 	while (1)
// 	{
// 		line = readline(prompt);
// 		if (line && *line)
// 		{
// 			add_history (line);
// 			// printf("%s\n", line);
// 		}
// 		// free(line);
// 	}

// }
#include "../include/msh.h"

char	*get_evnr(char **user, char **name, char **cwd)
{
	char	**ptr;
	char	**t_ptr;

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
		prompt = ft_strjoin(prompt, user);
		tmp[i++] = prompt;
		if (name)
		{
			prompt = ft_strjoin(prompt, "@");
			tmp[i++] = prompt;
			prompt = ft_strjoin(prompt, name);
			tmp[i++] = prompt;
		}
		if (cwd)
		{
			prompt = ft_strjoin(prompt, " ");
			tmp[i++] = prompt;
			prompt = ft_strjoin(prompt, cwd);
			tmp[i++] = prompt;
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

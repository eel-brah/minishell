#include "../include/msh.h"

// Variable names must start with a letter or underscore.
// Subsequent characters can be letters, digits, or underscores.

size_t	count_args(char **ptrs)
{
	char	**ptr;

	ptr = ptrs;
	while (ptr && *ptr)
		ptr++;
	return (ptr - ptrs);
}

void	ft_printenv(char **env)
{
	while (env && *env)
		printf("%s\n", *env++);
}

void	free_env(char **env)
{
	char	**ptr;

	ptr = env;
	while (ptr && *ptr)
		free(*ptr++);
	free(env);
}

bool	is_valid_variable(char *s)
{
	if (!s)
		return (false);
	if(!(ft_isalpha(*s) || *s == '_'))
		return (false);
	s++;
	while (*s && *s != '=')
	{
		if (!(ft_isalnum(*s) || *s == '_'))
			return (false);
		s++;
	}
	return (true);
}
bool	add_to_env(char ***env, char *vrbl)
{
	char	**new_env;
	char	**env_ptr;
	size_t	size;
	size_t	i;

	env_ptr = *env;
	size = count_args(env_ptr);
	new_env = malloc((size + 2) * sizeof(char *));
	if (!new_env)
	{
		perror("malloc");
		return (false);
	}
	i = 0;
	while (i < size)
	{
		new_env[i] = env_ptr[i];
		i++;
	}
	new_env[i] = vrbl;
	new_env[i + 1] = NULL;
	free(env_ptr);
	*env = new_env;
	return (true);
}

bool	edit_env(char ***env, char *vrbl)
{
	size_t	i;
	char	**env_ptr;
	char	*ptr;
	char	c;
	size_t	len;

	env_ptr = *env;
	if (is_valid_variable(vrbl))
	{
		i = 0;
		ptr = ft_strchr(vrbl, '=');
		while (env_ptr[i])
		{
			if (ptr)
			{
				if (!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl + 1))
				{
					free (env_ptr[i]);
					env_ptr[i] = vrbl;
					return (true);
				}
			}
			else if (!ft_strncmp(env_ptr[i], vrbl, ft_strlen(vrbl)) && env_ptr[i][ft_strlen(vrbl)] == '=')
				return (false);
			i++;
		}
		if (!add_to_env(env, vrbl))
			return (false);
	}
	else
	{
		printf("export: `%s': not a valid identifier\n", vrbl);
		return (false);
	}
	return (true);
}

bool	ft_export(char ***env, char **args)
{
	char	*ptr;

	if (!count_args(args))
		ft_printenv(*env);
	else
	{
		while (*args)
		{
			ptr = ft_strdup(*args);
			if (!ptr)
			{
				perror("malloc");
				return (false);
			}
			if (!edit_env(env, ptr))
			{
				free(ptr);
				return (false);
			}
			args++;
		}
	}
	return (true);
}

char	**create_env(char **env)
{
	size_t	size;
	char	**ptr;
	size_t	i;

	size = count_args(env);
	ptr = malloc((size + 1) * sizeof(env));
	if (!ptr)
	{
		perror("malloc");
		return (NULL);
	}
	i = 0;
	while (*env)
	{
		ptr[i++] = ft_strdup(*env++);
		if (!ptr[i - 1])
		{
			perror("malloc");
			free_env(ptr);
			return (NULL);
		}
	}
	ptr[i] = NULL;
	return (ptr);
}

void fu()
{
	system("leaks a.out");
}

int	main(int argc, char **argv, char **env)
{
	atexit(fu);
	char	**tmp;

	env = create_env(env);
	if (!env)
		return (1);
	ft_export(&env, argv+1);
	// env = tmp;
	ft_printenv(env);
	free_env(env);
}

#include "../include/msh.h"


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

char	**ft_env(char **env, char **args)
{
	char	**tmp_env;

	tmp_env = create_env(env);
	if (!tmp_env)
		return (NULL);
	while (args && *args)
	{
		if (**args == '=')
		{
			printf("env: setenv %s: Invalid argument\n", *args);
			free_env(tmp_env);
			return (NULL);
		}
		if (ft_strchr(*args, '='))
		{
			char	*ptr = ft_strdup(*args);
			if (!add_to_env(&tmp_env, ptr))
			{
				free_env(tmp_env);
				return (NULL);
			}
		}
		else
			return (tmp_env);
		args++;
	}
	ft_printenv(tmp_env);
	free_env(tmp_env);
	return (NULL);
}

int	main(int argc, char **argv, char **env)
{
	ft_env(env, argv + 1);
}
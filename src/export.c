#include "../include/msh.h"

// Variable names must start with a letter or underscore.
// Subsequent characters can be letters, digits, or underscores.

// size_t	count_args(char **ptrs)
// {
// 	char	**ptr;

// 	ptr = ptrs;
// 	while (ptr && *ptr)
// 		ptr++;
// 	return (ptr - ptrs);
// }

// void	ft_printenv(char **env)
// {
// 	while (env && *env)
// 		printf("%s\n", *env++);
// }

void	free_env(char **env)
{
	char	**ptr;

	ptr = env;
	while (ptr && *ptr)
		free(*ptr++);
	free(env);
}

int	is_valid_variable(char *s, int *plus)
{
	*plus = 0;
	if (!s)
		return (0);
	if(!(ft_isalpha(*s) || *s == '_'))
		return (0);
	s++;
	while (*s && *s != '=')
	{
		if ((*s == '+' && *(s + 1) == '='))
			return (*plus = 2, 2);
		if (!(ft_isalnum(*s) || *s == '_'))
			return (0);
		s++;
	}
	return (*plus=1, 1);
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
char	*alloc_without_plus(char *vrbl)
{
	char	*res;
	int		len;
	int		flag;
	int		i;
	int		index;

	flag = 1;
	i = 0;
	index = 0;
	len = ft_strlen(vrbl);
	res = malloc(sizeof(char) *len);
	while (vrbl[i])
	{
		if (vrbl[i] == '+' && flag)
		{
			flag = 0;
			i++;
		}
		res[index++] = vrbl[i++];
	}
	return (res);
}
bool	add_to_env_plus(char ***env, char *vrbl)
{
	char	**new_env;
	char	**env_ptr;
	size_t	size;
	size_t	i;

	env_ptr = *env;
	size = count_args(env_ptr);
	new_env = malloc((size + 2) * sizeof(char *));
	if (!new_env)
		return (perror("malloc"), false);
	i = 0;
	while (i < size)
	{
		new_env[i] = env_ptr[i];
		i++;
	}
	new_env[i] = alloc_without_plus(vrbl);
	if (new_env[i] == NULL)
		return (perror("malloc"), false);
	new_env[i + 1] = NULL;
	free(env_ptr);
	free(vrbl);
	*env = new_env;
	return (true);
}
char	*join_variabl(char *old, char *new, char *ptr)
{
	char	*tmp;
	char	*res;

	// sp = ft_split(new, '=');
	// if (!sp)
		// return (perror("malloc"), NULL);
	tmp = ft_strdup(ptr);
	if (!tmp)
		return (perror("malloc"), NULL);
	res = ft_strjoin(old, tmp);
	free(tmp);
	if (!res)
		return (perror("malloc"), NULL);
	free(old);
	free(new);
	return (res);
}

char	*edit_env(char ***env, char *vrbl)
{
	size_t	i;
	char	**env_ptr;
	char	*ptr;
	int		v;
	// size_t	len;

	env_ptr = *env;
	if (is_valid_variable(vrbl, &v))
	{
		i = 0;
		ptr = ft_strchr(vrbl, '=');
		// printf("prt %s\n",ptr);
		while (env_ptr[i])
		{
			if (ptr)
			{
				// printf("cc %c\n", vrbl[ptr - vrbl]);
				if (!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl + 1))
				{
					// printf("prt %s\n",ptr);
					// if (vrbl[ptr - vrbl - 1] != '+')
					// {
						free (env_ptr[i]);
						env_ptr[i] = vrbl;
						return ((char *)1337);
					// }
					// return ((char *)1337);
				}
				else if(!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl - 1) && vrbl[ptr - vrbl - 1] == '+' && vrbl[ptr - vrbl] == '=')
				{
					// free (env_ptr[i]);
					env_ptr[i] = join_variabl(env_ptr[i], vrbl, ptr + 1);
					if (env_ptr[i] == NULL)
						return (NULL);
					return ((char *)1337);
				}
			}
			else if (!ft_strncmp(env_ptr[i], vrbl, ft_strlen(vrbl)) && env_ptr[i][ft_strlen(vrbl)] == '=')
				return ((char *)1337);
			i++;
		}
		if (v == 1 && !add_to_env(env, vrbl))
			return (NULL);
		if (v == 2 && !add_to_env_plus(env, vrbl))
			return (NULL);
	}
	else
	{
		printf("export: `%s': not a valid identifier\n", vrbl);
		return ((char *)42);
	}
	return ((char *)1337);
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
			if (edit_env(env, ptr) == NULL)
			{
				free(ptr);
				return (false);
			}
			args++;
		}
	}
	return (true);
}

// char	**create_env(char **env)
// {
// 	size_t	size;
// 	char	**ptr;
// 	size_t	i;

// 	size = count_args(env);
// 	ptr = malloc((size + 1) * sizeof(env));
// 	if (!ptr)
// 	{
// 		perror("malloc");
// 		return (NULL);
// 	}
// 	i = 0;
// 	while (*env)
// 	{
// 		ptr[i++] = ft_strdup(*env++);
// 		if (!ptr[i - 1])
// 		{
// 			perror("malloc");
// 			free_env(ptr);
// 			return (NULL);
// 		}
// 	}
// 	ptr[i] = NULL;
// 	return (ptr);
// }

// void fu()
// {
// 	system("leaks a.out");
// }

// int	main(int argc, char **argv, char **env)
// {
// 	atexit(fu);
// 	char	**tmp;

// 	env = create_env(env);
// 	if (!env)
// 		return (1);
// 	ft_export(&env, argv+1);
// 	// env = tmp;
// 	ft_printenv(env);
// 	free_env(env);
// 	// printf("eenv %s, \n",getenv(argv[1]));
// 	// printf("eenv %s, \n",getenv(argv[2]));
// }

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
	if (!res)
		return (perror("malloc"), NULL);
	while (vrbl[i])
	{
		if (vrbl[i] == '+' && flag)
		{
			flag = 0;
			i++;
		}
		res[index++] = vrbl[i++];
	}
	res[index] = '\0';
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
	// export V / export V+= / export V=
	// V
	// B=
	// V=
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
				// printf("cc %c\n", vrbl[ptr - vrbl - 1]);
				if (!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl + 1) || (!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl) && env_ptr[i][ptr - vrbl] == '\0'))
				{
					free (env_ptr[i]);
					env_ptr[i] = vrbl;
					return ((char *)1337);
					
				}
				else if(!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl - 1) && vrbl[ptr - vrbl - 1] == '+')
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
			else if (!ft_strncmp(env_ptr[i], vrbl, ft_strlen(vrbl)) && env_ptr[i][ft_strlen(vrbl)] == '\0')
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
		print_error_2("export", vrbl, "not a valid identifier");
		return ((char *)42);
	}
	return ((char *)1337);
}

void	print_to(char *s, char *e)
{
	if (!s || !e || s > e)
		return ;
	while (s != e)
		ft_putchar_fd(*s++, 1);
}
void	print_sored_env(char **res)
{
	int		i;
	char	*eq;

	i = 0;
	while (res[i])
	{
		ft_putstr_fd("declare -x ", 1);
		eq = ft_strchr(res[i], '=');
		if (eq == NULL)
		{
			ft_putendl_fd(res[i++], 1);
			continue;
		}
		print_to(res[i], eq + 1);
		if (!eq[1])
			ft_putstr_fd("\"\"", 1);
		else
		{
			ft_putchar_fd('"', 1);
			print_to(eq + 1, &eq[ft_strlen(eq)]);
			ft_putchar_fd('"', 1);
		}
		ft_putchar_fd('\n', 1);
		i++;
	}
}

// void	print_sored_env(char **res)
// {
// 	int	i;
// 	int	j;
// 	int	flag;


// 	i = 0;
// 	while (res[i])
// 	{
// 		printf("declare -x ");
// 		j = 0;
// 		flag = 1;
// 		if (ft_strchr(res[i], '=') == NULL)
// 		{
// 			printf("%s\n", res[i]);
// 			i++;
// 			continue;
// 		}
// 		while (res[i][j])
// 		{
// 			if (res[i][j] == '=' && flag)
// 			{
// 				flag = 0;
// 				if (!res[i][j+1])
// 					printf("%c\"\"",res[i][j]);
// 				else
// 					printf("%c\"",res[i][j]);
// 				j++;
// 				continue;
// 			}
// 			if(res[i][j + 1] == '\0')
// 				printf("%c\"",res[i][j]);
// 			else
// 				printf("%c",res[i][j]);
// 			j++;
// 		}
// 		printf("\n");
// 		i++;
// 	}
// }

int	ft_printexport(char **env)
{
	int		size;
	char	**res;
	int		i;

	size = count_args(env);
	res = malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (perror("malloc"), 1);
	i = 0;
	while (i < size)
	{
		res[i] = env[i];
		i++;
	}
	res[i] = NULL;
	sort_2d_array(&res);
	print_sored_env(res);
	free(res);
	return (0);
}
int	ft_export(char ***env, char **args)
{
	char	*ptr;
	char	*tmp;
	int		r;

	if (!count_args(args))
		return (ft_printexport(*env));
	else
	{
		r = 0;
		while (*args)
		{
			ptr = ft_strdup(*args);
			if (!ptr)
				return (perror("malloc"), 1);
			tmp = edit_env(env, ptr);
			if (tmp == NULL)
				return (free(ptr), 1);
			else if (tmp == (char *)42)
				r = 1;
			args++;
		}
	}
	return (r);
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

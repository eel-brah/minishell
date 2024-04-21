#include "../include/msh.h"

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

bool	add_to_env(char *vrbl)
{
	char	**new_env;
	char	**env_ptr;
	size_t	size;
	size_t	i;

	env_ptr = environ;
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
	environ = new_env;
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

bool	add_to_env_plus(char *vrbl)
{
	char	**new_env;
	char	**env_ptr;
	size_t	size;
	size_t	i;

	env_ptr = environ;
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
	environ = new_env;
	return (true);
}

char	*join_variabl(char *old, char *new, char *ptr)
{
	char	*tmp;
	char	*res;

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

char	*edit_env(char *vrbl)
{
	size_t	i;
	char	**env_ptr;
	char	*ptr;
	int		v;

	env_ptr = environ;
	if (is_valid_variable(vrbl, &v))
	{
		i = 0;
		ptr = ft_strchr(vrbl, '=');
		while (env_ptr[i])
		{
			if (ptr)
			{
				if (!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl + 1) || (!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl) && env_ptr[i][ptr - vrbl] == '\0'))
				{
					free (env_ptr[i]);
					env_ptr[i] = vrbl;
					return ((char *)1337);
					
				}
				else if(!ft_strncmp(env_ptr[i], vrbl, ptr - vrbl - 1) && vrbl[ptr - vrbl - 1] == '+')
				{
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
		if (v == 1 && !add_to_env(vrbl))
			return (NULL);
		if (v == 2 && !add_to_env_plus(vrbl))
			return (NULL);
	}
	else
	{
		print_error_2("export", vrbl, "not a valid identifier");
		return ((char *)42);
	}
	return ((char *)1337);
}

void	substr_print(char *s, char *e)
{
	if (!s || !e || s > e)
		return ;
	while (s != e)
		ft_putchar_fd(*s++, 1);
	// script faildes in test () because ) print in stdoiut not stderr but if print to stderr some other test fail hhhh 
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
		substr_print(res[i], eq + 1);
		if (!eq[1])
			ft_putstr_fd("\"\"", 1);
		else
		{
			ft_putchar_fd('"', 1);
			substr_print(eq + 1, &eq[ft_strlen(eq)]);
			ft_putchar_fd('"', 1);
		}
		ft_putchar_fd('\n', 1);
		i++;
	}
}

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

int	ft_export(char **args)
{
	char	*ptr;
	char	*tmp;
	int		r;

	if (!count_args(args))
		return (ft_printexport(environ));
	else
	{
		r = 0;
		while (*args)
		{
			ptr = ft_strdup(*args);
			if (!ptr)
				return (perror("malloc"), 1);
			tmp = edit_env(ptr);
			if (tmp == NULL)
				return (free(ptr), 1);
			else if (tmp == (char *)42)
				r = 1;
			args++;
		}
	}
	return (r);
}

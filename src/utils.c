/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 03:31:48 by eel-brah          #+#    #+#             */
/*   Updated: 2024/04/22 18:32:52 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

size_t	count_args(char **ptrs)
{
	char	**ptr;

	ptr = ptrs;
	while (ptr && *ptr)
		ptr++;
	return (ptr - ptrs);
}

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

char	**ptrs_realloc(char **tokens, char *arg)
{
	size_t	size;
	char	**ntokens;

	if (!arg)
		return (NULL);
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

void	double_free(char **ptr)
{
	char	**tmp;

	tmp = ptr;
	while(tmp && *tmp)
		free(*tmp++);
	free(ptr);
}

void	print_error(char *source, char *error)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

void	syntax_error(char *source, char *error, char *start, char *end)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(" `", 2);
	if (start && !*start)
		ft_putstr_fd("newline", 2);
	else
		substr_print(start, end, 2);
	ft_putstr_fd("'\n", 2);
}

void	print_error_2(char *source, char *arg, char *error)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

bool	is_valid_variable_name(char *s)
{
	if (!s)
		return (false);
	if(!(ft_isalpha(*s) || *s == '_'))
		return (false);
	s++;
	while (*s)
	{
		if (!(ft_isalnum(*s) || *s == '_'))
			return (false);
		s++;
	}
	return (true);
}

char	*ft_strrealloc(char *str, size_t size)
{
	char	*new_str;

	new_str = malloc(size);
	if (!new_str)
		return (NULL);
	ft_strlcat(new_str, str, size);
	free(str);
	return (new_str);
}

int	nb_without_empty(char **env)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '=') != NULL)
			res++;
		i++;
	}
	return (res);
}

char	**env_without_empty(char **env)
{
	int		i;
	int		size;
	int		index;
	char	**res;

	size = nb_without_empty(env);
	res = malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	index = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '=') != NULL)
			res[index++] = env[i];
		i++;
	}
	res[index] = NULL;
	return (res); 
}

void	ft_printenv(char **env)
{
	while (env && *env)
		printf("%s\n", *env++);
}

void	ft_printenv_no_empty(char **env)
{
	while (env && *env)
	{
		if (ft_strchr(*env, '=') != NULL)
			printf("%s\n", *env);
		env++;
	}
}

char	*strdup_v2(char *start, char *end)
{
	char			*s;
	unsigned int	size;

	if (!start || !end || start > end)
		return (NULL);
	size = (end - start) + 1;
	s = malloc(size * sizeof(char));
	if (!s)
		return (NULL);
	ft_strlcpy(s, start, size);
	return (s);
}

int	exit_status(int	status, bool update, bool shift)
{
	static int s;

	if (update && shift)
		s = status << 8;
	else if (update)
		s = status;
	return (s);
}

bool	_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (true);
	return (false);
}

bool	_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (true);
	return (false);
}


bool	_isnumber(char *s)
{
	if (!s || !*s)
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	_isword(char *s)
{
	if (!s || !*s)
		return (false);
	while (*s)
	{
		if (!_isalpha(*s) && *s != '_')
			return (false);
	}
	return (true);
}

// bool	assignment_word(char *s)
// {
	
// }
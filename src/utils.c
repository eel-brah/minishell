/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 03:31:48 by eel-brah          #+#    #+#             */
/*   Updated: 2024/03/23 07:32:00 by eel-brah         ###   ########.fr       */
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
	while(*tmp)
		free(*tmp++);
	free(ptr);
}

void	print_error(char *source, char *error)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
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

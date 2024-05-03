/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_0.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:57:39 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:47:52 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static char	**mv_args_realloc(char **tokens, char **arg, int count, int index)
{
	size_t	size;
	char	**ntokens;
	char	**p2;
	char	**p;

	size = count_args(tokens);
	size += count;
	ntokens = malloc(sizeof(char *) * size);
	if (!ntokens)
		return (NULL);
	p2 = tokens;
	p = ntokens;
	ft_memcpy ((char *)ntokens, (char *)tokens, index * sizeof (char *));
	ntokens += index;
	tokens += index;
	free(*tokens++);
	ft_memcpy ((char *)ntokens, (char *)arg, count * sizeof (char *));
	ntokens += count;
	ft_memcpy ((char *)ntokens, (char *)tokens,
		(size - count - index - 1) * sizeof (char *));
	ntokens += (size - count - index - 1);
	*ntokens = NULL;
	free(p2);
	return (p);
}

static void	rm_ptr(char **args)
{
	int		j;
	char	*ptr;

	j = 0;
	ptr = args[j];
	while (args[j])
	{
		args[j] = args[j + 1];
		j++;
	}
	free(ptr);
}

static char	**mv_args(char ***args, char **ex,
		unsigned int count, unsigned int index)
{
	if (count == 1)
	{
		free((*args)[index]);
		(*args)[index] = *ex;
	}
	else if (count > 1)
	{
		*args = mv_args_realloc(*args, ex, count, index);
		if (!*args)
		{
			print_error_2("minishell", "malloc", strerror(errno));
			double_free(ex);
			return (NULL);
		}
	}
	return ((void *)1);
}

char	**expand_args(char **args)
{
	unsigned int	i;
	unsigned int	count;
	char			**ex;

	i = 0;
	while (args[i])
	{
		count = 0;
		ex = expander(args[i], 0, 1, GET_STAUS);
		if (!ex)
			return (NULL);
		count = count_args(ex);
		if (count == 0)
			rm_ptr(&args[i]);
		else
		{
			if (!mv_args(&args, ex, count, i))
				return (NULL);
		}
		i += count;
		free(ex);
	}
	return (args);
}

char	*expand_file(char *file)
{
	char	**ex;

	ex = expander(file, 0, 1, GET_STAUS);
	if (!ex)
		return (NULL);
	if (count_args(ex) == 1)
	{
		free(file);
		file = *ex;
		free(ex);
		return (file);
	}
	print_error_2("minishell", file, "ambiguous redirect");
	double_free(ex);
	return (NULL);
}

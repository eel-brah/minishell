/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:27:08 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/01 21:11:32 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*get_pid(int pid, int exitt)
{
	char	*file;
	char	*line;
	int		fd;

	file = name_file_rand((int)&get_pid);
	if (!file)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (free(file), perror("fork"), NULL);
	if (pid == 0)
		handle_child_get_pid(file);
	waitpid(pid, &exitt, 0);
	if (WEXITSTATUS(exitt) != 0)
		return (unlink(file), free(file), NULL);
	fd = open(file, O_CREAT | O_RDWR, PREMISSIONS);
	unlink(file);
	free(file);
	if (fd == -1)
		return (NULL);
	line = get_next_line(fd);
	close(fd);
	if (!line)
		return (NULL);
	return (line);
}

static char	*alloc(t_elem *elem, char **word, char ***res, int here_doc)
{
	char	**tmp;
	char	**concat;

	if (elem->wild && here_doc == 0)
	{
		concat = match_pattern(elem->arr, 1, 0, 1024);
		if (concat == (char **)42)
			*word = delete_quotes(elem->arr, 0, 0);
		else if (concat == NULL)
			return (perror("malloc"), free(elem->arr), d_free(*res), NULL);
		else
		{
			tmp = concat_two_array(*res, concat);
			if (tmp == NULL)
				return (perror("malloc"), free(elem->arr), d_free(*res), NULL);
			*res = tmp;
			elem->i = 0;
		}
		elem->wild = 0;
	}
	else if (here_doc == 0)
		*word = delete_quotes(elem->arr, 0, 0);
	else
		*word = ft_strdup(elem->arr);
	return ((char *)42);
}

char	*alloc_for_elem(t_elem *elem, int here_doc, char *word, char ***res)
{
	char	**tmp;
	char	**concat;

	tmp = NULL;
	concat = NULL;
	elem->i = 1;
	if (!alloc(elem, &word, res, here_doc))
		return (NULL);
	if (elem->i)
	{
		if (!word)
			return (perror("malloc"), free(elem->arr), d_free(*res), NULL);
		tmp = ft_realloc(*res, word);
		if (!tmp)
		{
			d_free(*res);
			return (free(word), free(elem->arr), perror("malloc"), NULL);
		}
		*res = tmp;
	}
	elem->index = 0;
	ft_memset(elem->arr, 0, elem->capacity);
	return ((char *)42);
}

char	*handle_wild_inside_expand(char *****res, char **sp, int i, char *tmp)
{
	char	**tmp1;
	char	**exp;

	exp = match_pattern(sp[i], 0, 0, 1024);
	if (!exp)
		return (d_free(sp), perror("malloc "), NULL);
	else if (exp == (char **)42)
	{
		tmp = ft_strdup(sp[i]);
		if (!tmp)
			return (free(sp), perror("malloc "), NULL);
		tmp1 = ft_realloc(***res, tmp);
		if (!tmp1)
			return (d_free(***res), d_free(sp), perror("malloc"), NULL);
		else
			***res = tmp1;
	}
	else
	{
		tmp1 = concat_two_array(***res, exp);
		if (tmp1 == NULL)
			return (perror("malloc "), d_free(sp), NULL);
		***res = tmp1;
	}
	return ((char *)42);
}

char	*handle_first_in_expand(t_elem **elem, char *s, int *start)
{
	(*elem)->i++;
	if (s[(*elem)->i] >= '0' && s[(*elem)->i] <= '9')
		return (s);
	*start = 0;
	if (s[(*elem)->i] == '\0')
	{
		(*elem)->i--;
		(*elem)->arr[(*elem)->index] = s[(*elem)->i];
		(*elem)->index++;
		return (s);
	}
	while (s[(*elem)->i + *start] && is_alpha_num(s[(*elem)->i + *start]))
		(*start)++;
	if ((*start == 0 && ((*elem)->qoute == 1
				|| (*elem)->here_doc)) || ((*elem)->expand == 0 && *start != 0))
	{
		(*elem)->i--;
		(*elem)->arr[(*elem)->index] = s[(*elem)->i];
		((*elem)->index)++;
		return (s);
	}
	else if ((*elem)->expand == 0 && *start == 0)
		return ((*elem)->i--, s);
	return (NULL);
}

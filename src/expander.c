/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:50:59 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 18:56:57 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*ft_strrealloc2(char *str, size_t size)
{
	char	*new_str;

	new_str = ft_calloc(size, sizeof(char));
	if (!new_str)
		return (NULL);
	ft_strlcat(new_str, str, size);
	free(str);
	return (new_str);
}

char	*pid_set(t_elem **elem)
{
	char	*pid;
	int		i;

	i = 0;
	pid = get_pid(0, 0);
	while (pid && pid[i] && pid[i] != '\n')
	{
		if (set_caractere(*elem, pid[i]) == NULL)
			return (free(pid), NULL);
		i++;
	}
	free(pid);
	return ((char *)42);
}

char	*home_set(t_elem **elem)
{
	char	*home;
	int		i;

	i = 0;
	home = getenv("HOME");
	if (home)
	{
		while (home && home[i])
		{
			if (set_caractere(*elem, home[i]) == NULL)
				return (NULL);
			i++;
		}
	}
	return ((char *)42);
}

char	*handle_dollar_special(char *s, t_elem **elem, int status)
{
	char	*num;
	int		j;

	j = 0;
	((*elem)->i)++;
	if (s[(*elem)->i] == '$')
		return (pid_set(elem));
	if (s[(*elem)->i] == '0')
		return (shell_name(elem));
	if (s[(*elem)->i] != '?')
	{
		if (set_caractere(*elem, s[(*elem)->i]) == NULL)
			return (NULL);
		return (s);
	}
	num = ft_itoa(status >> 8);
	if (!num)
		return (perror("malloc"), NULL);
	while (num[j])
	{
		if (set_caractere(*elem, num[j]) == NULL)
			return (NULL);
		j++;
	}
	return (free(num), s);
}

char	**expander(char *s, int here_doc, int expand, int status)
{
	char	**res;
	char	*word;
	t_elem	elem;

	if (intial_struct(&elem, &word, &res, here_doc) == NULL)
		return (NULL);
	(1) && (elem.expand = expand, elem.status = status);
	while (s && s[elem.i])
	{
		if ((s[elem.i] == '\'' || s[elem.i] == '\"' )
			&& (elem.qoute == 0 || elem.q == s[elem.i]))
		{
			if (update_quote(s, &elem) == NULL)
				return (free(elem.arr), d_free(res), NULL);
		}
		else
		{
			if (handle_o_char_sp(&elem, s, word, &res) == NULL)
				return (NULL);
		}
		elem.i++;
	}
	if (handle_last(&elem, &res, word) == NULL)
		return (NULL);
	return (free(elem.arr), res);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:11:42 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 13:30:38 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	**concat_two_array(char **res, char **concat)
{
	char	**result;
	int		len;
	int		i;

	len = 0;
	while (res && res[len])
		len++;
	i = 0;
	while (concat && concat[i])
		i++;
	result = malloc(sizeof(char *) * (len + i + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (res && res[i])
		(1) && (result[i] = res[i], i++);
	len = 0;
	while (concat && concat[len])
	{
		result[i] = concat[len];
		len++;
		i++;
	}
	return (free(res), free(concat), result[i] = NULL, result);
}

char	*handle_dollar(char *s, char ****res, t_elem **elem)
{
	int		j;
	char	*exp;
	char	*env;
	int		start;

	if (handle_first_in_expand(elem, s, &start) != NULL)
		return (s);
	exp = malloc(start + 1);
	if (!exp)
		return (NULL);
	j = 0;
	while (j < start)
	{
		exp[j] = s[(*elem)->i + j];
		j++;
	}
	exp[j] = '\0';
	((*elem)->i) = ((*elem)->i) + start - 1;
	env = getenv(exp);
	free(exp);
	if (handle_env_in_expand(env, elem, s) != NULL)
		return (s);
	if (split_expand(elem, s, res) == NULL)
		return (NULL);
	return (s);
}

char	*handle_expand_without_wild(char **sp, char *****res, int i)
{
	char	*tmp;
	char	**tmp1;

	tmp = ft_strdup(sp[i]);
	if (!tmp)
		return (free(sp), perror("malloc "), NULL);
	tmp1 = ft_realloc(***res, tmp);
	if (!tmp1)
	{
		d_free(***res);
		return (***res = NULL, d_free(sp), perror("malloc "), NULL);
	}
	else
		***res = tmp1;
	return ((char *)42);
}

char	*handle_wild_in_dollar(char *arr, char *****res)
{
	char	**sp;
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	sp = ft_split(arr, ' ');
	free(arr);
	if (!sp)
		return (NULL);
	while (sp[i])
	{
		if (ft_strchr(sp[i], '*'))
		{
			if (handle_wild_inside_expand(res, sp, i, tmp) == NULL)
				return (NULL);
		}
		else
		{
			if (handle_expand_without_wild(sp, res, i) == NULL)
				return (NULL);
		}
		i++;
	}
	return (d_free(sp), (char *)42);
}

char	*handle_env_in_expand(char *env, t_elem **elem, char *s)
{
	int		j;

	if (!env)
	{
		if (!is_alpha_num(s[(*elem)->i + 1]) && s[(*elem)->i + 1] != '\0'
			&& s[(*elem)->i] == '$'
			&& s[(*elem)->i + 1] != '\'' && s[(*elem)->i + 1] != '"')
			(*elem)->arr[((*elem)->index)++] = s[(*elem)->i];
		return (s);
	}
	j = 0;
	if ((*elem)->here_doc == 0 && set_caractere(*elem, 20) == NULL)
		return (NULL);
	while (env[j])
	{
		if (set_caractere(*elem, env[j]) == NULL)
			return (NULL);
		j++;
	}
	if ((*elem)->here_doc == 0 && set_caractere(*elem, 20) == NULL)
		return (NULL);
	return (NULL);
}

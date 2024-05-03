/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:18:45 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:37:00 by eel-brah         ###   ########.fr       */
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

void	double_free(char **ptr)
{
	char	**tmp;

	tmp = ptr;
	while (tmp && *tmp)
		free(*tmp++);
	free(ptr);
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

void	ft_printenv(char **env)
{
	while (env && *env)
		printf("%s\n", *env++);
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

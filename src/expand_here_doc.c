/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:37:43 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 16:37:59 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

int	expand_here_doc(int fd, int status, int expand)
{
	char	*s;
	int		count;
	char	*name;
	char	**res;
	int		fd_res;
	int		fd_ret;

	count = 69;
	name = ft_itoa((int)&expand_here_doc);
	if (!name)
		return (perror("malloc"), -1);
	fd_res = open(name, O_CREAT | O_RDWR | O_TRUNC | O_EXCL, PREMISSIONS);
	while (fd_res == -1  && errno == EEXIST)
	{
		free(name);
		name = ft_itoa((int)&expand_here_doc + count);
		if (!name)
			return (perror("malloc"), -1);
		fd_res = open(name, O_CREAT | O_RDWR | O_TRUNC | O_EXCL, PREMISSIONS);
		count++;
	}
	fd_ret = open(name, O_RDWR, PREMISSIONS);
	unlink(name);
	if (fd_res == -1 || fd_ret == -1)
		return (perror("open"), free(name), close(fd_res), close(fd_ret), -1);
	while (1)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		res = expander(s, 1, expand, status);
		if (!res)
			return (close(fd_res), close(fd_ret), free(name), free(s), -1);
		write(fd_res, *res, ft_strlen(*res));
		double_free(res);
		free(s);
	}
	return (close(fd_res), free(name), fd_ret);
}

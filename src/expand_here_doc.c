/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:37:43 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/01 21:13:30 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

int	vo_name(int fd, int expand, int status, int fd_res)
{
	char	*s;
	char	**res;

	while (1)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		res = expander(s, 1, expand, status);
		if (!res)
			return (free(s), 0);
		write(fd_res, *res, ft_strlen(*res));
		double_free(res);
		free(s);
	}
	return (1);
}

int	expand_here_doc(int fd, int status, int expand)
{
	int		count;
	char	*name;
	int		fd_res;
	int		fd_ret;

	(1) && (name = name_file_rand((int)&expand_here_doc), count = 42);
	if (!name)
		return (-1);
	fd_res = open(name, O_CREAT | O_RDWR | O_TRUNC | O_EXCL, PREMISSIONS);
	while (fd_res == -1 && errno == EEXIST)
	{
		free(name);
		name = name_file_rand((int)&expand_here_doc + count);
		if (!name)
			return (-1);
		fd_res = open(name, O_CREAT | O_RDWR | O_TRUNC | O_EXCL, PREMISSIONS);
		count++;
	}
	fd_ret = open(name, O_RDWR, PREMISSIONS);
	unlink(name);
	if (fd_res == -1 || fd_ret == -1)
		return (perror("open"), free(name), close(fd_res), close(fd_ret), -1);
	if (!vo_name(fd, expand, status, fd_res))
		return (close(fd_res), close(fd_ret), free(name), -1);
	return (close(fd_res), free(name), fd_ret);
}

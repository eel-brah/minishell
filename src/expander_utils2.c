/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:27:08 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/25 18:29:47 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*get_pid(int pid, int exitt)
{
	char	*file;
	char	*line;
	int		fd;

	file = ft_itoa((int)&get_pid);
	if (!file)
		return (perror("malloc"), NULL);
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

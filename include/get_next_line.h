/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:18:50 by eel-brah          #+#    #+#             */
/*   Updated: 2024/04/04 02:14:27 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>

typedef struct s_buffer
{
	char	*buf;
	size_t	i;
}	t_buffer;

char	*get_next_line(int fd);
char	*ft_gnl_get_line(t_buffer *buffer, int fd, int rt);
char	*ft_gnl_generate_line(char **line, size_t r, size_t i, char **buf);
char	*ft_gnl_free(char **buf, char *line, char cs);
size_t	ft_gnl_find_nl(size_t i, char *buf, int *rt);

#endif
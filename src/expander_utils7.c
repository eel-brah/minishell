/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:12:15 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/14 14:50:42 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*name_file_rand(int number)
{
	char	*name;
	char	*tmp;

	tmp = ft_itoa(number);
	if (!tmp)
		return (perror("malloc"), NULL);
	name = ft_strjoin("/tmp/", tmp);
	free(tmp);
	if (!name)
		return (perror("malloc"), NULL);
	return (name);
}

void	print_error_3(char *source, char *arg, char *arg2, char *error)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg2, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

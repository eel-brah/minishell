/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:24:03 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/11 17:58:13 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	in_pipe(int i)
{
	static int	p;

	if (i == 0)
		p = 0;
	else if (i == 1)
		p = 1;
	return (p);
}

bool	is_valid_variable_name(char *s)
{
	if (!s)
		return (false);
	if (!(ft_isalpha(*s) || *s == '_'))
		return (false);
	s++;
	while (*s)
	{
		if (!(ft_isalnum(*s) || *s == '_'))
			return (false);
		s++;
	}
	return (true);
}

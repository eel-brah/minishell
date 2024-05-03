/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_t.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 03:31:48 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:27:35 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (true);
	return (false);
}

bool	_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (true);
	return (false);
}


bool	_isnumber(char *s)
{
	if (!s || !*s)
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	_isword(char *s)
{
	if (!s || !*s)
		return (false);
	while (*s)
	{
		if (!_isalpha(*s) && *s != '_')
			return (false);
	}
	return (true);
}

// bool	assignment_word(char *s)
// {
	
// }





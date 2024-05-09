/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setupe_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:13:16 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/09 15:13:30 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

void	check_path_pwd(char **env, size_t *i, int *add)
{
	*add = 0;
	*i = 0;
	if (env_is_there(env, "PATH") == false)
		(*add)++;
	if (env_is_there(env, "PWD") == false)
		(*add)++;
	if (env_is_there(env, "SHLVL") == false)
		(*add)++;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:29:49 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/13 21:44:49 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

void	and_type(t_div *div)
{
	execute(div->left);
	if (!exit_status(0, false, false))
		execute(div->right);
}

void	or_type(t_div *div)
{
	execute(div->left);
	if (exit_status(0, false, false))
		execute(div->right);
}

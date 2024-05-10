/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:11:03 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/10 19:27:30 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	reset_term()
{
	struct termios	attr;

	if (tcgetattr(0, &attr) == -1)
		return (perror("tcgetattr"), false);
	attr.c_cflag = TTYDEF_CFLAG;
	attr.c_iflag = TTYDEF_IFLAG;
	attr.c_lflag = TTYDEF_LFLAG;
	attr.c_oflag = TTYDEF_OFLAG;
	if (tcsetattr(0, 0, &attr) == -1)
		return (perror("tcsetattr"), false);
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:11:03 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/13 21:34:34 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	reset_term(void)
{
	struct termios	attr;

	if (tcgetattr(STDIN_FILENO, &attr) == -1)
		return (perror("tcgetattr"), false);
	attr.c_cflag = TTYDEF_CFLAG;
	attr.c_iflag = TTYDEF_IFLAG;
	attr.c_lflag = TTYDEF_LFLAG;
	attr.c_oflag = TTYDEF_OFLAG;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &attr) == -1)
		return (perror("tcsetattr"), false);
	return (true);
}

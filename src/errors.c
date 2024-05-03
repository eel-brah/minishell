/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:52:47 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 21:19:58 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

t_node	*invalid_red(t_node *node, char *st, char *et)
{
	exit_status(258, true, true);
	syntax_error("minishell",
		"syntax error near unexpected token", st, et);
	free_cmdtree(node);
	return (NULL);
}

t_node	*check_exec(t_exec *cmd, t_node *node, char *pcmd)
{
	char	*st;
	char	*et;

	if (!cmd->argv && node == (t_node *)cmd)
	{
		free_cmdtree(node);
		exit_status(258, true, true);
		get_token(&pcmd, &st, &et);
		syntax_error("minishell", "syntax error near unexpected token", st, et);
		return (NULL);
	}
	return (node);
}

void	print_error(char *source, char *error)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

void	syntax_error(char *source, char *error, char *start, char *end)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(" `", 2);
	if (start && !*start)
		ft_putstr_fd("newline", 2);
	else
		substr_print(start, end, 2);
	ft_putstr_fd("'\n", 2);
}

void	print_error_2(char *source, char *arg, char *error)
{
	ft_putstr_fd(source, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

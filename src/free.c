/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 12:16:26 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:49:12 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static void	free_exec(t_exec *cmd)
{
	double_free(cmd->argv);
	free(cmd);
}

static void	free_redirection(t_redirection *red)
{
	free(red->file);
	free_cmdtree((t_node *)red->node);
	free(red);
}

static void	free_heredoc(t_redirection *red)
{
	close(red->here_fd);
	free_cmdtree((t_node *)red->node);
	free(red);
}

static void	free_div(t_div *div)
{
	free_cmdtree((t_node *)div->left);
	free_cmdtree((t_node *)div->right);
	free(div);
}

void	free_cmdtree(t_node *tree)
{
	if (!tree)
		return ;
	if (tree->type == EXEC)
		free_exec((t_exec *)tree);
	else if (tree->type == RED)
		free_redirection((t_redirection *)tree);
	else if (tree->type == HEREDOC)
		free_heredoc((t_redirection *)tree);
	else if (tree->type == PIPE || tree->type == AND || tree->type == OR)
		free_div((t_div *)tree);
}

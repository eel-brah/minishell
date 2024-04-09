/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:41 by eel-brah          #+#    #+#             */
/*   Updated: 2024/04/08 22:12:23 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

volatile sig_atomic_t got_sigint = 0;
extern char **environ;

void	free_cmdtree(t_node *tree)
{
	t_exec			*cmd;
	t_redirection	*red;
	t_div			*div;

	if (!tree)
		return ;
	if (tree->type == EXEC)
	{
		cmd = (t_exec *)tree;
		double_free(cmd->argv);
		free(cmd);
	}
	else if (tree->type == RED)
	{
		red = (t_redirection *)tree;
		free(red->file);
		free_cmdtree((t_node *)red->node);
		free(red);
	}
	else if (tree->type == HEREDOC)
	{
		red = (t_redirection *)tree;
		// free(red->file);
		close(red->here_fd);
		free_cmdtree((t_node *)red->node);
		free(red);
	}
	else if (tree->type == PIPE)
	{
		div = (t_div *)tree;
		free_cmdtree((t_node *)div->left);
		free_cmdtree((t_node *)div->right);
		free(div);
	}
	else if (tree->type == AND)
	{
		div = (t_div *)tree;
		free_cmdtree((t_node *)div->left);
		free_cmdtree((t_node *)div->right);
		free(div);
	}
	else if (tree->type == OR)
	{
		div = (t_div *)tree;
		free_cmdtree((t_node *)div->left);
		free_cmdtree((t_node *)div->right);
		free(div);
	}
}

void fu()
{
	system("leaks minishell");
}

char	*get_cmd(char *prompt)
{
	char	*cmd;

	if (!prompt)
		cmd = readline("$ ");
	else
		cmd = readline(prompt);
	free(prompt);
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd;
	t_node	*tree;

	if (!setup(argc, argv, env))
		exit(1);
	while (1)
	{
		cmd = get_cmd(get_prompt());
		if (!cmd)
			ft_exit(NULL, NULL);
		else if (!*cmd)
		{
			free(cmd);
			continue;
		}
		tree = parse_cmd(cmd);
		free(cmd);
		if (!tree)
			continue;
		execute(tree);
		free_cmdtree(tree);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:41 by eel-brah          #+#    #+#             */
/*   Updated: 2024/04/22 18:52:15 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

volatile sig_atomic_t got_sigint = 0;
extern char **environ;

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

void	pre_trv(t_node *node) 
{
	if (node->type == EXEC || node->type == RED)
	{
		t_node *ptr = node;
		while (ptr->type == RED)
		{
			printf("RED %s\n", ((t_redirection *)ptr)->file);
			ptr = ((t_redirection *)ptr)->node;
		}
		printf("EXEX: %s\n", ((t_exec *)ptr)->argv[0]);
		return ;
	}
	else
		printf("%i\n", node->type);
	pre_trv(((t_div *)node)->left);
	pre_trv(((t_div *)node)->right);
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

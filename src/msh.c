/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:41 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/11 03:24:58 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

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
			continue ;
		}
		tree = parse_cmd(cmd);
		free(cmd);
		if (!tree)
			continue ;
		execute(tree);
		free_cmdtree(tree);
	}
}

static char	*get_cmd(char *prompt)
{
	char	*cmd;
	char	**env;

	env = environ;
	if (!prompt)
		cmd = readline("$ ");
	else
		cmd = readline(prompt);
	fix_env(env);
	free(prompt);
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}

void	execute(t_node *node)
{
	bool	b;

	b = true;
	if (node->type == EXEC)
		exec_type(node);
	else if (node->type == HEREDOC)
		b = heredoc_type((t_redirection *)node);
	else if (node->type == RED)
		b = red_type((t_redirection *)node);
	else if (node->type == PIPE)
		b = pipe_type((t_div *)node);
	else if (node->type == AND)
		and_type((t_div *)node);
	else if (node->type == OR)
		or_type((t_div *)node);
	if (!b)
		exit_status(1, true, true);
}

int	exit_status(int status, bool update, bool shift)
{
	static int	s;

	if (update && shift)
		s = status << 8;
	else if (update)
		s = status;
	return (s);
}

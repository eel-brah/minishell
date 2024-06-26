/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:46:24 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:58:18 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

t_node	*parse_heredoc(t_node *node, char *start, char *end)
{
	t_redirection	*red;
	int				fd;

	red = malloc(sizeof(*red));
	if (!red)
		return (NULL);
	red->fd = 0;
	red->type = HEREDOC;
	red->flags = O_RDWR | O_CREAT | O_TRUNC;
	red->expand = true;
	if (!open_herdoc_file(red, node, &fd))
		return (NULL);
	if (!fill_file_heredoc(red, strdup_v2(start, end), fd))
	{
		red->node = node;
		free_cmdtree((t_node *)red);
		return (NULL);
	}
	return (add_redirection(red, node));
}

static t_node	*parse_pipe(char **pcmd)
{
	t_node	*node;

	node = parse_exec(pcmd);
	if (!node)
		return (NULL);
	if (token_peek(*pcmd) == PIPELINE)
	{
		get_token(pcmd, 0, 0);
		node = diversion(node, parse_pipe(pcmd), PIPE);
		if (!node)
			return (NULL);
	}
	return (node);
}

static t_node	*pars_or(char **pcmd)
{
	t_node	*node;

	node = parse_pipe(pcmd);
	if (!node)
		return (NULL);
	if (token_peek(*pcmd) == TOR)
	{
		get_token(pcmd, 0, 0);
		node = diversion(node, pars_or(pcmd), OR);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_node	*pars_and(char **pcmd)
{
	t_node	*node;

	node = pars_or(pcmd);
	if (!node)
		return (NULL);
	if (token_peek(*pcmd) == TAND)
	{
		get_token(pcmd, 0, 0);
		node = diversion(node, pars_and(pcmd), AND);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_node	*parse_cmd(char *cmd)
{
	t_node	*tree;
	char	*st;
	char	*et;

	while (*cmd && ft_strchr(WHITESPACES, *cmd))
		cmd++;
	if (!*cmd)
		return (NULL);
	tree = pars_and(&cmd);
	if (!tree)
		return (NULL);
	if (get_token(&cmd, &st, &et))
	{
		exit_status(258, true, true);
		syntax_error("minishell", "syntax error near unexpected token", st, et);
		free_cmdtree(tree);
		return (NULL);
	}
	return (tree);
}

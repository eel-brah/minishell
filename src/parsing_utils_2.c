#include "../include/msh.h"

t_node	*parse_redirection(t_node *node, char **pcmd)
{
	char	red;
	char	*st;
	char	*et;
	char	token;

	token = token_peek(*pcmd);
	while (token == OUT_RED || token == IN_RED || token == APP_RED || token == HERE_DOC)
	{
		red = get_token(pcmd, 0, 0);
		token = get_token(pcmd, &st, &et);
		if (token != WORD)
		{
			exit_status(258, true, true);
			print_error("minishell", "syntax error");
			free_cmdtree(node);
			return (NULL);
		}
		if (red != HERE_DOC)
			node = parse_redirection_create(st, et, node, red);
		else
			node = parse_heredoc(node, st, et);
		if (!node)
			return (NULL);
		token = token_peek(*pcmd);
	}
	return (node);
}

t_node	*parse_parenthesis(char **pcmd)
{
	t_node	*node;

	get_token(pcmd, NULL, NULL);
	node = pars_and(pcmd);
	if (!node)
		return (NULL);
	if (token_peek(*pcmd) != CLOSE_PER)
	{
		free_cmdtree(node);
		exit_status(258, true, true);
		print_error("minishell", "syntax error 4");
		return (NULL);
	}
	get_token(pcmd, NULL, NULL);
	return (parse_redirection(node, pcmd));
}

char	**fill_argv(char token, char **pcmd, t_node *node, t_exec *cmd)
{
	char	*st;
	char	*et;
	char	**tmp;

	if (token != WORD)
	{
		exit_status(258, true, true);
		print_error("minishell", "syntax error");
		free_cmdtree(node);
		return (NULL);
	}
	get_token(pcmd, &st, &et);
	tmp = ptrs_realloc(cmd->argv, strdup_v2(st, et));
	if (!tmp)
	{
		free_cmdtree(node);
		return (NULL);
	}
	cmd->argv = tmp;
	return (tmp);
}

t_node	*check_exec(t_exec *cmd, t_node *node)
{
	if (!cmd->argv && node == (t_node *)cmd)
	{
		free_cmdtree(node);
		exit_status(258, true, true);
		print_error("minishell", "syntax error");
		return (NULL);
	}
	return (node);
}

t_node	*parse_exec(char **pcmd)
{
	t_exec	*cmd;
	t_node	*node;
	char	token;

	if(token_peek(*pcmd) == OPEN_PER)
		return (parse_parenthesis(pcmd));
	cmd = create_exec();
	if (!cmd)
		return (NULL);
	node = parse_redirection((t_node *)cmd, pcmd);
	if (!node)
		return (NULL);
	token = token_peek(*pcmd);
	while (token && token != PIPELINE && token != OAND && token != OOR && token != CLOSE_PER)
	{
		if (!fill_argv(token, pcmd, node, cmd))
			return (NULL);
		node = parse_redirection(node, pcmd);
		if (!node)
			return (NULL);
		token = token_peek(*pcmd);
	}
	return (check_exec(cmd, node));
}

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
			// print_error("minishell", "syntax error 2");
			syntax_error("minishell", "syntax error near unexpected token", st, et); 
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
	if (**pcmd)
	{
		node = pars_and(pcmd);
		if (!node)
			return (NULL);
	}
	if (token_peek(*pcmd) != CLOSE_PER)
	{
		free_cmdtree(node);
		exit_status(258, true, true);
		print_error_2("minishell", "syntax error", "unclosed parenthesis");
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

	get_token(pcmd, &st, &et);
	if (token != WORD)
	{
		exit_status(258, true, true);
		// print_error("minishell", "syntax error 5");
		if (token == OPEN_PER)
			get_token(pcmd, &st, &et);
		syntax_error("minishell", "syntax error near unexpected token", st, et); 
		free_cmdtree(node);
		return (NULL);
	}
	tmp = ptrs_realloc(cmd->argv, strdup_v2(st, et));
	if (!tmp)
	{
		free_cmdtree(node);
		return (NULL);
	}
	cmd->argv = tmp;
	return (tmp);
}

t_node	*check_exec(t_exec *cmd, t_node *node, char *pcmd)
{
	char	*st;
	char	*et;

	if (!cmd->argv && node == (t_node *)cmd)
	{
		free_cmdtree(node);
		exit_status(258, true, true);
		// print_error("minishell", "syntax error 3");
		get_token(&pcmd, &st, &et);
		syntax_error("minishell", "syntax error near unexpected token", st, et);
		return (NULL);
	}
	return (node);
}

t_node	*parse_exec(char **pcmd)
{
	t_exec	*cmd;
	t_node	*node;
	char	token;
	char	*st;
	char	*et;

	if(token_peek(*pcmd) == OPEN_PER)
	{
		node = parse_parenthesis(pcmd);
		char token = token_peek(*pcmd);
		if (node && (token == WORD || token == ERROR))
		{
			exit_status(258, true, true);
			get_token(pcmd, &st, &et);
			syntax_error("minishell", "syntax error near unexpected token", st, et);
			free_cmdtree(node);
			return (NULL);
		}
		return (node);
	}
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
	return (check_exec(cmd, node, *pcmd));
}

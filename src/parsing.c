#include "../include/msh.h"

extern volatile sig_atomic_t got_sigint;

bool	get_token_cmp2(char *p, char *r)
{
	if (!ft_strncmp(p, "&&", 2))
		*r = OAND;
	else if (!ft_strncmp(p, "||", 2))
		*r = OOR;
	else if (!ft_strncmp(p, "<<", 2))
		*r = HERE_DOC;
	else if (!ft_strncmp(p, ">>", 2))
		*r = APP_RED;
	else
		return (false);
	return (true);
}

bool	get_token_cmp1(char *p, char *r)
{
	if (*p == '|')
		*r = PIPELINE;
	else if (*p == '>')
		*r = OUT_RED;
	else if (*p == '<')
		*r = IN_RED;
	else if (*p == '(')
		*r = OPEN_PER;
	else if (*p == ')')
		*r = CLOSE_PER;
	else
		return (false);
	return (true);
}

void	get_token_word(char **dp, char *r, bool print)
{
	char	*p;
	bool	Q;
	char	q;

	Q = false;
	p = *dp;
	while (*p && ((!ft_strchr(WHITESPACES, *p) && (!ft_strchr(SYMBOL, *p)
		|| (*p == '&' && *(p + 1) != '&'))) || Q))
	{
		if ((*p == '\"' || *p == '\'') && (!Q || (Q && *p == q)))
		{
			q = *p;
			Q = !Q;
		}
		p++;
	}
	if (Q)
	{
		if (print)
			print_error("minishell", "syntax error");
		*r = ERROR;
	}
	else
		*r = WORD;
	*dp = p;
}

char	get_token(char **s, char **st, char **et)
{
	char	*p;
	char	r;

	p = *s;
	while (*p && ft_strchr(WHITESPACES, *p))
		p++;
	if (st)
		*st = p;
	if (!*p)
		r = 0;
	else if (get_token_cmp2(p, &r))
		p += 2;
	else if (get_token_cmp1(p, &r))
		p += 1;
	else
		get_token_word(&p, &r, 0);
	if (et)
		*et = p;
	*s = p;
	return (r);
}


char	token_peek(char *p)
{
	char	r;

	while (*p && ft_strchr(WHITESPACES, *p))
		p++;
	if (!*p)
		r = 0;
	else if (get_token_cmp2(p, &r))
		p += 2;
	else if (get_token_cmp1(p, &r))
		p += 1;
	else
		get_token_word(&p, &r, false);
	return (r);
}

t_exec	*create_exec()
{
	t_exec	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->type = EXEC;
	cmd->argv = NULL;
	return (cmd);
}

t_node	*diversion(t_node *left, t_node *right, int token)
{
	t_div *node;

	if (!right)
	{
		free_cmdtree(left);
		return (NULL);
	}
	node = malloc(sizeof(*node));
	if (!node)
	{
		free_cmdtree(right);
		free_cmdtree(left);
		return (NULL);
	}
	node->type = token;
	node->left = left;
	node->right = right;
	return ((t_node*)node);
}


t_node	*add_redirection(t_redirection *red, t_node *node)
{
	t_redirection	*ptr;
	t_redirection	*prev;

	red->node = node;
	if ((node->type == RED || node->type == HEREDOC) && ((t_redirection *)node)->fd == red->fd)
	{
		ptr = (t_redirection *)node;
		while ((ptr->type == RED || ptr->type == HEREDOC) && ptr->fd == red->fd)
		{
			prev = ptr;
			ptr = (t_redirection *)ptr->node;
		}
		red->node = (t_node *)ptr;
		prev->node = (t_node *)red;
		return (node);
	}
	return (t_node *)red;
}

t_node	*create_redirection(t_node *node, char *file, int flags, int fd)
{
	t_redirection	*red;

	if (!file)
		return (NULL);
	red = malloc(sizeof(*red));
	if (!red)
		return (NULL);
	red->type = RED;
	red->file = file;
	red->flags = flags;
	red->fd = fd;
	return (add_redirection(red, node));
}

bool	fill_file_heredoc(t_redirection *node, char *delim, int fd)
{
	char	*s;
	char	**res;
	int		len;
	int		len_s;

	if (!delim)
		return (false);
	if (ft_strchr(delim, '\'') || ft_strchr(delim, '"'))
		node->expand = false;
	res = expander(delim, 0, 0, 0);
	free(delim);
	if (!res)
	{
		perror("malloc");
		return (false);
	}
	delim = res[0];
	len = ft_strlen(delim);
	// fd = dup(node->here_fd); // check fail of dup
	set_signal_handler(SIGINT, sigint_handler2);
	// signal(SIGINT, sigint_handler2);
	int fd2 = dup(0);
	while (1)
	{
		s = readline("> ");
		if (got_sigint)
		{
			dup2(fd2, 0);
			close(fd2);
			double_free(res);
			free(s);
			close(fd);
			got_sigint = 0;
			set_signal_handler(SIGINT, sigint_handler);
			return (false);
		}
		len_s = ft_strlen(s);
		if (!s || (len == len_s && !ft_strncmp(s, delim, len)))
			break ;
		write(fd, s, len_s);
		write(fd, "\n", 1);
		free(s);
	}
	set_signal_handler(SIGINT, sigint_handler);
	double_free(res);
	free(s);
	close(fd);
	close(fd2);
	return (!!1337);
}

t_node	*parse_heredoc(t_node *node, char *start, char *end)
{
	t_redirection	*red;
	char	*file = "smo_0098731277";
	int		fd;

	red = malloc(sizeof(*red));
	if (!red)
		return (NULL);
	red->fd = 0;
	red->type = HEREDOC;
	red->file = file;
	red->flags = O_RDWR|O_CREAT|O_TRUNC;
	red->here_fd = open(file, red->flags, PREMISSIONS);
	fd = open(file, red->flags, PREMISSIONS);
	if (red->here_fd == -1 || fd == -1)
	{
		// close 
		red->node = node;
		free_cmdtree((t_node *)red);
		perror("open");
		return (NULL);
	}
	unlink(file);
	red->expand = true;
	if (!fill_file_heredoc(red, strdup_v2(start, end), fd))
	{
		red->node = node;
		free_cmdtree((t_node *)red);
		return (NULL);
	}
	return (add_redirection(red, node));
}

t_node	*parse_redirection_create(char *st, char *et, t_node *node, char red)
{
	char	*file;
	t_node	*tmp_node;

	file = strdup_v2(st, et);
	tmp_node = node;
	if (red == IN_RED)
		node = create_redirection(node, file, O_RDONLY, 0);
	else if (red == OUT_RED)
		node = create_redirection(node, file, O_WRONLY|O_CREAT|O_TRUNC, 1);
	else if (red == APP_RED)
		node = create_redirection(node, file, O_WRONLY|O_CREAT|O_APPEND, 1);
	if (!node)
	{
		free(file);
		free_cmdtree(tmp_node);
		return (NULL);
	}
	return (node);
}

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
			if (token != ERROR)
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

t_node	*pars_and(char **pcmd);

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
		print_error("minishell", "syntax error 4");
		return (NULL);
	}
	get_token(pcmd, NULL, NULL);
	return (parse_redirection(node, pcmd));
}

t_node	*parse_exec(char **pcmd)
{
	t_exec	*cmd;
	t_node	*node;
	char	*st;
	char	*et;
	char	token;
	char	**tmp;

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
		if (token != WORD)
		{
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
		node = parse_redirection(node, pcmd);
		if (!node)
			return (NULL);
		token = token_peek(*pcmd);
	}
	if (!cmd->argv && node == (t_node *)cmd)
	{
		free_cmdtree(node);
		print_error("minishell", "syntax error");
		return (NULL);
	}
	return (node);
}

t_node	*parse_pipe(char **pcmd)
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

t_node	*pars_or(char **pcmd)
{
	t_node	*node;

	node = parse_pipe(pcmd);
	if (!node)
		return (NULL);
	if (token_peek(*pcmd) == OOR)
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
	if (token_peek(*pcmd) == OAND)
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

	while (*cmd && ft_strchr(WHITESPACES, *cmd))
		cmd++;
	if (!*cmd)
		return (NULL);
	tree = pars_and(&cmd);
	if (!tree)
		return (NULL);
	while (*cmd && ft_strchr(WHITESPACES, *cmd))
		cmd++;
	if (*cmd)
	{
		print_error("minishell", "syntax error");
		free_cmdtree(tree);
		return (NULL);
	}
	return (tree);
}


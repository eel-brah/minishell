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

void	get_token_word(char **dp, char *r)
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
		*r = ERROR;
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
		get_token_word(&p, &r);
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
		get_token_word(&p, &r);
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
	ft_memset(red, 0, sizeof(*red));
	red->type = RED;
	red->file = file;
	red->flags = flags;
	red->fd = fd;
	return (add_redirection(red, node));
}

void	close_heredoc(int fd, int fd2, char *s, char **res)
{
	close(fd);
	close(fd2);
	free(s);
	double_free(res);
	set_signal_handler(SIGINT, sigint_handler);
	got_sigint = 0;
}

char	**heredoc_setup(char *delim, t_redirection *node, unsigned int *len, int *fd2)
{
	char	**res;

	if (!delim)
	{
		perror("malloc");
		return (NULL);
	}
	*fd2 = dup(0);
	if (*fd2 == -1)
	{
		perror("dup");
		return (NULL);
	}
	if (ft_strchr(delim, '\'') || ft_strchr(delim, '"'))
		node->expand = false;
	res = expander(delim, 0, 0, 0);
	free(delim);
	if (!res)
	{
		perror("malloc");
		return (NULL);
	}
	*len = ft_strlen(res[0]);
	return (res);
}

void	write_line_to_file(int fd, char *s, unsigned int len_s)
{
	write(fd, s, len_s);
	write(fd, "\n", 1);
}

bool	_reset(int fd, int fd2, char *s, char **res)
{
	set_signal_handler(SIGINT, SIG_IGN);
	if (dup2(fd2, 0) == -1)
		perror("dup2");
	close_heredoc(fd, fd2, s, res);
	return (false);
}

bool	fill_file_heredoc(t_redirection *node, char *delim, int fd)
{
	char			*s;
	char			**res;
	unsigned int	len;
	unsigned int	len_s;
	int				fd2;

	res = heredoc_setup(delim, node, &len, &fd2);
	if (!res)
		return (false);
	set_signal_handler(SIGINT, sigint_handler2);
	while (1)
	{
		s = readline("> ");
		if (got_sigint)
			return (_reset(fd, fd2, s, res));
		len_s = ft_strlen(s);
		if (!s || (len == len_s && !ft_strncmp(s, res[0], len)))
			break ;
		write_line_to_file(fd, s, len_s);
		free(s);
		s = NULL;
	}
	close_heredoc(fd, fd2, s, res);
	return (!!1337);
}

bool	open_herdoc_file(t_redirection *red, t_node *node, int *fd)
{
	char	*file = "smo_0098731277";

	red->file = file;
	red->here_fd = open(file, red->flags, PREMISSIONS);
	*fd = open(file, red->flags, PREMISSIONS);
	unlink(file);
	if (red->here_fd == -1 || *fd == -1)
	{
		close(*fd);
		close(red->here_fd);
		red->node = node;
		free_cmdtree((t_node *)red);
		perror("open");
		return (false);
	}
	return (true);
}

t_node	*parse_heredoc(t_node *node, char *start, char *end)
{
	t_redirection	*red;
	int				fd;

	red = malloc(sizeof(*red));
	if (!red)
		return (NULL);
	red->fd = 0;
	red->type = HEREDOC;
	red->flags = O_RDWR|O_CREAT|O_TRUNC;
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
		exit_status(258, true, true);
		print_error("minishell", "syntax error");
		free_cmdtree(tree);
		return (NULL);
	}
	return (tree);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:41 by eel-brah          #+#    #+#             */
/*   Updated: 2024/04/05 23:07:03 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

volatile sig_atomic_t got_sigint = 0;

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

bool	_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (true);
	return (false);
}

bool	_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (true);
	return (false);
}


bool	_isnumber(char *s)
{
	if (!s || !*s)
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	_isword(char *s)
{
	if (!s || !*s)
		return (false);
	while (*s)
	{
		if (!_isalpha(*s) && *s != '_')
			return (false);
	}
	return (true);
}

// bool	assignment_word(char *s)
// {
	
// }

char	*get_cmd(char *prompt)
{
	char	*cmd;

	if (!prompt)
		prompt = "$";
	cmd = readline(prompt);
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
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

bool	peek(char **pcmd, char *set)
{
	char *ptr;

	if (!pcmd || !*pcmd)
		return (false);
	ptr = *pcmd;
	while(*ptr && ft_strchr(WHITESPACES, *ptr))
		ptr++;
	*pcmd = ptr;
	return (*ptr && ft_strchr(set, *ptr));
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
	signal(SIGINT, sigint_handler2);
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
			signal(SIGINT, sigint_handler);
			return (false);
		}
		len_s = ft_strlen(s);
		if (!s || (len == len_s && !ft_strncmp(s, delim, len)))
			break ;
		write(fd, s, len_s);
		write(fd, "\n", 1);
		free(s);
	}
	signal(SIGINT, sigint_handler);
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
	t_node	*tmp;

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
	tmp = node;
	node = parse_redirection(node, pcmd);
	return (node);
}

void sigint_handler(int sig)
{
	exit_status(1, true, true);
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void sigint_handler2(int sig)
{
	exit_status(1, true, true);
	got_sigint = 1;
	close(0);
	(void)sig;
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

// void	parse_cmd_term(t_node *node)
// {
// 	t_exec			*exec;
// 	t_div			*dev;
// 	t_redirection	*red;
// 	int				i;

// 	i = 0;
// 	if (node->type == EXEC)
// 	{
// 		exec = (t_exec*)node;
// 		while (exec->argv && exec->argv[i]) // > file
// 			*(exec->eargv[i++]) = 0;
// 	}
// 	else if (node->type == RED)
// 	{
// 		red = (t_redirection *)node;
// 		*(red->efile) = 0;
// 		parse_cmd_term(red->node);
// 	}
// 	else if (node->type == PIPE)
// 	{
// 		dev = (t_div*)node;
// 		parse_cmd_term(dev->left);
// 		parse_cmd_term(dev->right);
// 	}
// 	else if (node->type == AND)
// 	{
// 		dev = (t_div*)node;
// 		parse_cmd_term(dev->left);
// 		parse_cmd_term(dev->right);
// 	}
// 	else if (node->type == OR)
// 	{
// 		dev = (t_div*)node;
// 		parse_cmd_term(dev->left);
// 		parse_cmd_term(dev->right);
// 	}
// }

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

char	**ptrs_realloc2(char **tokens, char **arg, int count, int index)
{
	size_t	size;
	char	**ntokens;

	size = 0;
	while (tokens[size])
			size++;
	size += count;
	ntokens = malloc(sizeof(char *) * size);
	if (!ntokens)
		return (NULL);
	// int j = 0;
	// int i = 0;
	// char k[] = "0123456";
	// char **p1 = arg;
	char **p2 = tokens;
	char **p = ntokens;
	ft_memcpy ((char *) ntokens, (char *) tokens, index * sizeof (char *));
	ntokens += index;
	tokens += index;
	free(*tokens++);
	ft_memcpy ((char *) ntokens, (char *) arg, count * sizeof (char *));
	ntokens += count;
	ft_memcpy ((char *) ntokens, (char *) tokens, (size - count - index - 1) * sizeof (char *));
	ntokens += (size - count - index - 1);
	// while (tokens[j] && index != j)
	// 	ntokens[i++] = tokens[j++];
	// int k = 0;
	// while (arg[k])
	// 	ntokens[i++] = arg[k++];
	// free(tokens[j++]);
	// while (tokens[j])
	// 	ntokens[i++] = tokens[j++];
	*ntokens = NULL;
	// free(p1);
	free(p2);
	return (p);
}

void	rm_ptr(char **args)
{
	int		j;
	char	*ptr;

	j = 0;
	ptr = args[j];
	while (args[j])
	{
		args[j] = args[j + 1];
		j++;
	}
	free(ptr);
}

char	**expand_args(char **args)
{
	unsigned int	i;
	unsigned int	count;
	char			**ex;

	i = 0;
	while (args[i])
	{
		count = 0;
		ex = expander(args[i], 0, 1, exit_status(0, false, false));
		if (!ex)
			return (NULL);
		count = count_args(ex);
		if (count == 1)
		{
			free(args[i]);
			args[i] = *ex;
		}
		else if (count > 1)
		{
			args = ptrs_realloc2(args, ex, count, i);
			if (!args)
				return (NULL);
		}
		else
			rm_ptr(&args[i]);
		i += count;
		free(ex);
	}
	return (args);
}

char	*expand_file(char *file)
{
	char **ex;
	int	count;

	count = 0;
	ex = expander(file, 0, 1, exit_status(0, false, false));
	if (!ex)
		return (NULL);
	while(ex[count])
		count++;
	if (count == 1)
	{
		free(file);
		file = *ex;
		free(ex);
		return(file);
	}
	print_error_2("minishell", file, "ambiguous redirect");
	double_free(ex);
	return (NULL);
}

// system calls returns
void	execute(t_node *node, char ***env)
{
	t_exec			*cmd;
	t_redirection	*red;
	t_div			*div;
	pid_t			pid[3];
	int				p[2];
	int				or;
	char **tmp;
	
	if (node->type == EXEC)
	{
		cmd = (t_exec *)node;
		if (cmd->argv)
		{
			tmp = expand_args(cmd->argv);
			if (!tmp)
			{
				exit_status(1, true, true);
				return ;
			}
			cmd->argv = tmp;
			exec_cmd(node, cmd->argv[0], cmd->argv, env);
			// if (WIFSIGNALED(*status))
			// || WTERMSIG(*status) == SIGQUIT
			int s = exit_status(0, false, false);
			if (WTERMSIG(s) == SIGINT)
			{
				exit_status(130, true, true);
				ft_putchar_fd('\n', 1);
			}
		}
		else
			exit_status(0, true, true);
	}
	else if (node->type == HEREDOC)
	{
		red = (t_redirection*)node;
		int fd = dup(red->fd);
		int fd_read = expand_here_doc(red->here_fd, exit_status(0, false, false), red->expand);
		close(red->here_fd);
		// check error and free
		if (fd_read == -1)
			return ;
		red->here_fd = fd_read;
		dup2(red->here_fd, red->fd);
		close(red->here_fd);
		execute(red->node, env);
		dup2(fd, red->fd);
		close(fd);
	}
	else if (node->type == RED)
	{
		red = (t_redirection*)node;
		char *tmp2;
		tmp2 = expand_file(red->file);
		if (!tmp2)
		{
			exit_status(1, true, true);
			return ;
		}
		red->file = tmp2;
		// S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); /* rw-rw-rw- 
		or = open(red->file, red->flags, PREMISSIONS);
		if(or < 0)
		{
			exit_status(1, true, true);
			perror("open");
			return ;
		}
		// if signal arrive while this happning
		int fd = dup(red->fd);
		dup2(or, red->fd);
		close(or);
		execute(red->node, env);
		dup2(fd, red->fd);
		close(fd);
	}
	else if (node->type == PIPE)
	{
		div = (t_div*)node;
		if(pipe(p) < 0)
		{
			perror("pipe");
			return ;
		}
		signal(SIGINT, SIG_IGN);
		pid[0] = fork();
		if (pid[0] < 0)
		{
			perror("fork");
			return ;
		}
		if (!pid[0])
		{
			dup2(p[1], 1);
			close(p[1]);
			close(p[0]);
			execute(div->left, env);
			int s = exit_status(0, false, false);
			exit(WEXITSTATUS(s));
		}
		pid[1] = fork();
		if (pid[1] < 0)
		{
			// status
			perror("fork");
			return ;
		}
		if (!pid[1])
		{
			dup2(p[0], 0);
			close(p[1]);
			close(p[0]);
			execute(div->right, env);
			int s = exit_status(0, false, false);
			exit(WEXITSTATUS(s));
		}
		close(p[0]);
		close(p[1]);
		int s;
		while (waitpid(pid[0], &s, 0) == -1)
		{
			// status
			if (errno != EINTR) 
				return ;
		}
		while (waitpid(pid[1], &s, 0) == -1)
		{
			if (errno != EINTR) 
				return ;
		}
		exit_status(s, true, false);
		signal(SIGINT, sigint_handler);
	}
	else if (node->type == AND)
	{
		div = (t_div*)node;
		execute(div->left, env);
		if (!exit_status(0, false, false))
			execute(div->right, env);
	}
	else if (node->type == OR)
	{
		div = (t_div*)node;
		execute(div->left, env);
		if (exit_status(0, false, false))
			execute(div->right, env);
	}
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

char	**create_new_env()
{
	char	**env;
	char	*tmp;
	char	*tmp1;

	env = malloc(sizeof(char *) * 5);
	if (!env)
		return (perror("malloc"), NULL);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), NULL);
	tmp1 = ft_strdup(tmp);
	env[0] = tmp1;
	tmp = ft_strdup("SHLVL=1");
	if (!tmp || !tmp1)
		return (perror("malloc"), NULL);
	env[1]= tmp;
	tmp = ft_strdup("_=/usr/bin/env"); // handle it
	if (!tmp)
		return (perror("malloc"), NULL);
	env[2]= tmp;
	tmp = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	if (!tmp)
		return (perror("malloc"), NULL); // free the previous ones
	env[3]= tmp;
	return (env[4]= NULL, env);
}

char	**create_env(char **env)
{
	size_t	size;
	char	**ptr;
	size_t	i;

	size = count_args(env);
	if (size == 0)
		return (create_new_env());
	ptr = malloc((size + 1) * sizeof(env));
	if (!ptr)
		return (perror("malloc"), NULL);
	i = 0;
	while (*env)
	{
		ptr[i++] = ft_strdup(*env++);
		if (!ptr[i - 1])
		{
			perror("malloc");
			double_free(ptr);
			return (NULL);
		}
	}
	ptr[i] = NULL;
	return (ptr);
}

void fu()
{
	system("leaks minishell");
}

extern int rl_catch_signals;
extern char **environ;

int	exit_status(int	status, bool update, bool shift)
{
	static int s;

	if (update && shift)
		s = status << 8;
	else if (update)
		s = status;
	return (s);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd;
	t_node	*tree;
	char	*prompt;
	char	**_env;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);

	(void) argv;
	if (argc > 1)
		print_error("minishell", "Invalid number of arguments");
	_env = create_env(env);
	if (!_env)
		return (1);
	rl_catch_signals = 0;
	environ = _env;
	while (1)
	{
		prompt = get_prompt();
		cmd = get_cmd(prompt);
		free(prompt);
		if (!cmd)
			ft_exit(NULL, NULL, _env);
		else if (!*cmd)
		{
			free(cmd);
			continue;
		}
		tree = parse_cmd(cmd);
		free(cmd);
		if (!tree)
			continue;
		execute(tree, &_env);
		free_cmdtree(tree);
	}
	double_free(_env);
	// rl_clear_history();
}


// | && || cmd > < >> <<

// A simple command is a sequence of optional parameter assignments followed by blank-separated words, with optional redirections interspersed.
// A pipeline is either a simple command, or a sequence of two or more simple commands where each command is separated from the next by '|'
// A sublist is either a single pipeline, or a sequence of two or more pipelines separated by '&&' or '||'.

// pcmd = cmd;
// ecmd = cmd + ft_strlen(cmd);
// while (*pcmd)
// {
// 	printf("%c: ", get_token(&pcmd, ecmd, &st, &et));
// 	fflush(stdout);
// 	while (st < et)
// 	{
// 		write (1, st, 1);
// 		st++;
// 	}
// 	write (1, "\n", 1);
// }
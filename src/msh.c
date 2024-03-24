/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:41 by eel-brah          #+#    #+#             */
/*   Updated: 2024/03/24 10:13:49 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	get_token_cmp2(char *p, char *r)
{
	if (!ft_strncmp(p, "&&", 2))
		*r = AND;
	else if (!ft_strncmp(p, "||", 2))
		*r = OR;
	else if (!ft_strncmp(p, "<<", 2))
		*r = HEREDOC;
	else if (!ft_strncmp(p, ">>", 2))
		*r = APPEND_REDIRECTION;
	else
		return (false);
	return (true);
}

bool	get_token_cmp1(char *p, char *r)
{
	if (*p == '|')
		*r = PIPELINE;
	else if (*p == '>')
		*r = OUTPUT_REDIRECTION;
	else if (*p == '<')
		*r = INPUT_REDIRECTION;
	else if (*p == '(')
		*r = OPEN_PER;
	else if (*p == ')')
		*r = CLOSE_PER;
	else
		return (false);
	return (true);
}

char	get_token(char **s, char **st, char **et)
{
	char	*p;
	char	r;
	bool	Q;
	char	q;

	p = *s;
	Q = false;
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
	{
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
			ft_putendl_fd("Synax errur 4", 2);
			return ('e');
		}
		r = WORD;
	}
	if (et)
		*et = p;
	*s = p;
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
	{
		add_history(cmd); // return on error
		// char *ptr = cmd;
		cmd = ft_strtrim(cmd, WHITESPACES);
		// free(ptr);
		if (!cmd)
			perror("malloc");
		return (cmd);
	}
	free (cmd);
	return (NULL);
}

t_exec	*create_exec()
{
	t_exec	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->type = EXEC;
	cmd->argv = NULL;
	cmd->eargv = NULL;
	return (cmd);
}

t_node	*create_pipe(t_node *left, t_node *right)
{
	t_pipe *pipe;

	if (!right)
	{
		free_cmdtree(left);
		return (NULL);
	}
	pipe = malloc(sizeof(*pipe));
	if (!pipe)
	{
		free_cmdtree(right);
		free_cmdtree(left);
		return (NULL);
	}
	pipe->type = PIPE;
	pipe->left = left;
	pipe->right = right;
	return ((t_node*)pipe);
}

t_node	*add_redirection(t_redirection *red, t_node *node)
{
	t_redirection	*ptr;
	t_redirection	*prev;

	red->node = node;
	if (node->type == RED && ((t_redirection *)node)->fd == red->fd)
	{
		ptr = (t_redirection *)node;
		while (ptr->type == RED && ptr->fd == red->fd)
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

t_node	*create_redirection(t_node *node, char *file, char *efile, int flags, int fd)
{
	t_redirection	*red;

	red = malloc(sizeof(*red));
	if (!red)
		return (NULL);
	red->type = RED;
	red->file = file;
	red->efile = efile;
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

t_node	*parse_redirection(t_node *node, char **pcmd)
{
	char	red;
	char	*st;
	char	*et;

	while (peek(pcmd, "<>"))
	{
		red = get_token(pcmd, 0, 0);
		if (red == HEREDOC || get_token(pcmd, &st, &et) != WORD)
		{
			ft_putendl_fd("Syntax error 3", 2);
			return (NULL);
		}
		if (red == INPUT_REDIRECTION)
			node = create_redirection(node, st, et, O_RDONLY, 0);
		else if (red == OUTPUT_REDIRECTION)
			node = create_redirection(node, st, et, O_WRONLY|O_CREAT|O_TRUNC, 1);
		else if (red == APPEND_REDIRECTION)
			node = create_redirection(node, st, et, O_WRONLY|O_CREAT|O_APPEND, 1);
	}
	return (node);
}

t_node	*parse_exec(char **pcmd)
{
	t_exec	*cmd;
	t_node	*node;
	t_node	*tmp;
	char	*st;
	char	*et;
	char	r;

	cmd = create_exec();
	if (!cmd)
		return (NULL);
	node = parse_redirection((t_node *)cmd, pcmd);
	if (!node)
	{
		free_cmdtree((t_node *)cmd);
		return (NULL);
	}
	while (!peek(pcmd, "|"))
	{
		r = get_token(pcmd, &st, &et);
		if (r == 'e')
		{
			free_cmdtree(node);
			return (NULL);
		}
		if (r == 0)
			break;
		if (r != WORD)
		{
			free_cmdtree(node);
			ft_putendl_fd("Syntax error 1", 2);
			return (NULL);
		}
		cmd->argv = ptrs_realloc(cmd->argv, st);
		cmd->eargv = ptrs_realloc(cmd->eargv, et);
		if (!cmd->argv || !cmd->eargv)
		{
			free_cmdtree(node);
			return (NULL);
		}
		tmp = node;
		node = parse_redirection(node, pcmd);
		if (!node)
		{
			free_cmdtree(tmp);
			return (NULL);
		}
	}
	if (!cmd->argv && node == (t_node *)cmd)
	{
		free_cmdtree(node);
		ft_putendl_fd("Syntax error 2", 2);
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
	if (peek(pcmd, "|"))
	{
		get_token(pcmd, 0, 0);
		node = create_pipe(node, parse_pipe(pcmd));
		if (!node)
			return (NULL);
	}
	return (node);
}

void	parse_cmd_term(t_node *node)
{
	t_exec			*exec;
	t_pipe			*pipe;
	t_redirection	*red;
	int				i;

	i = 0;
	if (node->type == EXEC)
	{
		exec = (t_exec*)node;
		while (exec->argv && exec->argv[i]) // > file
			*(exec->eargv[i++]) = 0;
	}
	else if (node->type == RED)
	{
		red = (t_redirection *)node;
		*(red->efile) = 0;
		parse_cmd_term(red->node);
	}
	else if (node->type == PIPE)
	{
		pipe = (t_pipe*)node;
		parse_cmd_term(pipe->left);
		parse_cmd_term(pipe->right);
	}
}

t_node	*parse_cmd(char *cmd)
{
	t_node	*tree;

	tree = parse_pipe(&cmd);
	if (!tree)
		return (NULL);
	parse_cmd_term(tree);
	return (tree);
}

// system calls returns
void	execute(t_node *node, char **env)
{
	t_exec			*cmd;
	t_redirection	*red;
	t_pipe			*pipeline;
	pid_t			pe[2];
	int				p[2];
	int				r;
	int				or;
	
	if (node->type == EXEC)
	{
		cmd = (t_exec *)node;
		if (cmd->argv)
			exec_cmd(cmd->argv[0], cmd->argv, env);
	}
	else if (node->type == RED)
	{
		red = (t_redirection*)node;
		or = open(red->file, red->flags, PREMISSIONS);
		if(or < 0)
		{
			perror("open");
			exit(1);
		}
		dup2(or, red->fd);
		execute(red->node, env);
	}
	else if (node->type == PIPE)
	{
		pipeline = (t_pipe*)node;
		if(pipe(p) < 0)
		{
			perror("pipe");
			exit(1);
		}
		pe[0] = fork();
		if (pe[0] < 0)
		{
			perror("fork");
			exit(1);
		}
		if (!pe[0])
		{
			dup2(p[1], 1);
			close(p[1]);
			close(p[0]);
			execute(pipeline->left, env);
		}
		pe[1] = fork();
		if (pe[1] < 0)
		{
			perror("fork");
			exit(1);
		}
		if (!pe[1])
		{
			dup2(p[0], 0);
			close(p[1]);
			close(p[0]);
			execute(pipeline->right, env);
		}
		close(p[0]);
		close(p[1]);
		waitpid(pe[0], &r, 0);
		waitpid(pe[1], &r, 0);
	}
	exit(0);
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
	else if (node->type == PIPE)
		printf("PIPE\n");
	pre_trv(((t_pipe *)node)->left);
	pre_trv(((t_pipe *)node)->right);
}

void fu()
{
	system("leaks a.out");
}

void	free_cmdtree(t_node *tree)
{
	t_exec			*cmd;
	t_redirection	*red;
	t_pipe			*pipeline;

	if (!tree)
		return ;
	if (tree->type == EXEC)
	{
		cmd = (t_exec *)tree;
		free(cmd->argv);
		free(cmd->eargv);
		free(cmd);
	}
	else if (tree->type == RED)
	{
		red = (t_redirection *)tree;
		free_cmdtree((t_node *)red->node);
		free(red);
	}
	else if (tree->type == PIPE)
	{
		pipeline = (t_pipe *)tree;
		free_cmdtree((t_node *)pipeline->left);
		free_cmdtree((t_node *)pipeline->right);
		free(pipeline);
	}
}

char	**create_env(char **env)
{
	size_t	size;
	char	**ptr;
	size_t	i;

	size = count_args(env);
	ptr = malloc((size + 1) * sizeof(env));
	if (!ptr)
	{
		perror("malloc");
		return (NULL);
	}
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

int	main(int argc, char **argv, char **env)
{
	char	*cmd;
	t_node	*tree;
	int		pid;
	int	r;
	char	*prompt;
	char	**_env;

	(void) argc;
	(void) argv;
	// _env = create_env(env);
	// if (!_env)
	// 	return (1);
	_env = env;
	// atexit(fu);
	while (1)
	{
		prompt = get_prompt();
		cmd = get_cmd(prompt);
		if (!cmd)
			continue;
		tree = parse_cmd(cmd);
		if (!tree)
		{
			free(cmd);
			free(prompt);
			continue;
		}
		pid = fork();
		if (pid < 0)
		{
			free_cmdtree(tree);
			free(cmd);
			free(prompt);
			perror("fork");
			continue;
		}
		if(pid == 0)
			execute(tree, _env);
		wait(&r);
		free_cmdtree(tree);
		free(cmd);
		free(prompt);
	}
	// double_free(_env);
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
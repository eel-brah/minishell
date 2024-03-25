/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:41 by eel-brah          #+#    #+#             */
/*   Updated: 2024/03/25 15:49:02 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	get_token_cmp2(char *p, char *r)
{
	if (!ft_strncmp(p, "&&", 2))
		*r = OAND;
	else if (!ft_strncmp(p, "||", 2))
		*r = OOR;
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

char	token_peek(char *p)
{
	char	r;
	bool	Q;
	char	q;

	Q = false;
	while (*p && ft_strchr(WHITESPACES, *p))
		p++;
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
		// // char *ptr = cmd;
		// cmd = ft_strtrim(cmd, WHITESPACES);
		// // free(ptr);
		// if (!cmd)
		// 	perror("malloc");
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

t_node	*create_redirection(t_node *node, char *file, int flags, int fd)
{
	t_redirection	*red;

	if (file)
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

t_node	*parse_redirection(t_node *node, char **pcmd)
{
	char	red;
	char	*st;
	char	*et;
	char	*file;

	while (peek(pcmd, "<>"))
	{
		red = get_token(pcmd, 0, 0);
		if (red == HEREDOC || get_token(pcmd, &st, &et) != WORD)
		{
			ft_putendl_fd("Syntax error 3", 2);
			return (NULL);
		}
		file = strdup_v2(st, et);
		if (red == INPUT_REDIRECTION)
			node = create_redirection(node, file, O_RDONLY, 0);
		else if (red == OUTPUT_REDIRECTION)
			node = create_redirection(node, file, O_WRONLY|O_CREAT|O_TRUNC, 1);
		else if (red == APPEND_REDIRECTION)
			node = create_redirection(node, file, O_WRONLY|O_CREAT|O_APPEND, 1);
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
	// char	*s;

	cmd = create_exec();
	if (!cmd)
		return (NULL);
	node = parse_redirection((t_node *)cmd, pcmd);
	if (!node)
	{
		free_cmdtree((t_node *)cmd);
		return (NULL);
	}
	while (token_peek(*pcmd) != PIPELINE && token_peek(*pcmd) != OAND && token_peek(*pcmd) != OOR)
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
		cmd->argv = ptrs_realloc(cmd->argv, strdup_v2(st, et));
		if (!cmd->argv)
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

	tree = pars_and(&cmd);
	if (!tree)
		return (NULL);
	// parse_cmd_term(tree);
	return (tree);
}

// system calls returns
void	execute(t_node *node, char **env)
{
	t_exec			*cmd;
	t_redirection	*red;
	t_div			*div;
	pid_t			pid[3];
	int				p[2];
	static int		r;
	int				or;
	
	if (node->type == EXEC)
	{
		cmd = (t_exec *)node;
		if (cmd->argv)
		{
			// built in exit status && built in in pipeline
			r = built_in(cmd->argv[0], cmd->argv, env);
			if (r != -1)
				return ;
			pid[2] = fork();
			if (pid[2] == 0)
				exec_cmd(cmd->argv[0], cmd->argv, env);
			waitpid(pid[2], &r, 0);
		}
	}
	else if (node->type == RED)
	{
		red = (t_redirection*)node;
		or = open(red->file, red->flags, PREMISSIONS);
		if(or < 0)
		{
			perror("open");
			return ;
		}
		dup2(or, red->fd);
		execute(red->node, env);
	}
	else if (node->type == PIPE)
	{
		div = (t_div*)node;
		if(pipe(p) < 0)
		{
			perror("pipe");
			return ;
		}
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
			exit(r >> 8);
		}
		pid[1] = fork();
		if (pid[1] < 0)
		{
			perror("fork");
			return ;
		}
		if (!pid[1])
		{
			dup2(p[0], 0);
			close(p[1]);
			close(p[0]);
			execute(div->right, env);
			exit(r >> 8);
		}
		close(p[0]);
		close(p[1]);
		waitpid(pid[0], &r, 0);
		waitpid(pid[1], &r, 0);
	}
	else if (node->type == AND)
	{
		div = (t_div*)node;
		execute(div->left, env);
		if (!r)
			execute(div->right, env);
	}
	else if (node->type == OR)
	{
		div = (t_div*)node;
		execute(div->left, env);
		if (r)
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
	t_div			*pipeline;

	if (!tree)
		return ;
	if (tree->type == EXEC)
	{
		cmd = (t_exec *)tree;
		// free(cmd->argv);
		// free(cmd->eargv);
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
	else if (tree->type == PIPE)
	{
		pipeline = (t_div *)tree;
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
	char	*prompt;
	char	**_env;

	(void) argv;
	if (argc > 1)
		print_error("minishell", "Invalid number of arguments");
	_env = create_env(env);
	if (!_env)
		return (1);
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
		execute(tree, _env);
		// pre_trv(tree);
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
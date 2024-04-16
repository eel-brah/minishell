#include "../include/msh.h"

char	**ptrs_realloc2(char **tokens, char **arg, int count, int index)
{
	size_t	size;
	char	**ntokens;
	char	**p2;
	char	**p;

	size = count_args(tokens);
	size += count;
	ntokens = malloc(sizeof(char *) * size);
	if (!ntokens)
		return (NULL);
	p2 = tokens;
	p = ntokens;
	ft_memcpy ((char *)ntokens, (char *)tokens, index * sizeof (char *));
	ntokens += index;
	tokens += index;
	free(*tokens++);
	ft_memcpy ((char *)ntokens, (char *)arg, count * sizeof (char *));
	ntokens += count;
	ft_memcpy ((char *)ntokens, (char *)tokens,
		(size - count - index - 1) * sizeof (char *));
	ntokens += (size - count - index - 1);
	*ntokens = NULL;
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

char	**mv_args(char ***args, char **ex, unsigned int count, unsigned int index)
{
	if (count == 1)
	{
		free((*args)[index]);
		(*args)[index] = *ex;
	}
	else if (count > 1)
	{
		*args = ptrs_realloc2(*args, ex, count, index);
		if (!*args)
		{
			print_error_2("minishell", "malloc", strerror(errno));
			double_free(ex);
			return (NULL);
		}
	}
	return ((void *)1);
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
		if (count == 0)
			rm_ptr(&args[i]);
		else
		{
			if (!mv_args(&args, ex, count, i))
				return (NULL);
		}
		i += count;
		free(ex);
	}
	return (args);
}

char	*expand_file(char *file)
{
	char	**ex;

	ex = expander(file, 0, 1, exit_status(0, false, false));
	if (!ex)
		return (NULL);
	if (count_args(ex) == 1)
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

void		signal_exting(int s)
{
	if (WTERMSIG(s) == SIGINT)
	{
		exit_status(130, true, true);
		ft_putchar_fd('\n', 1);
	}
	else if (WTERMSIG(s) == SIGQUIT)
	{
		exit_status(131, true, true);
		ft_putstr_fd("Quit: 3\n", 1);
	}
}

void	exec_type(t_node *node)
{
	t_exec	*cmd;
	char	**tmp;

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
		exec_cmd(node, cmd->argv[0], cmd->argv);
		signal_exting(exit_status(0, false, false));
	}
	else
		exit_status(0, true, true);
}

bool	heredoc_type(t_redirection *red)
{
	int	fd;
	int	fd_read;

	if (red->expand == true)
	{
		fd_read = expand_here_doc(red->here_fd, exit_status(0, false, false), red->expand);
		if (fd_read == -1)
			return (false);
		close(red->here_fd);
		red->here_fd = fd_read;
	}
	fd = dup(red->fd);
	if (fd == -1)
		return (perror("dup"), false);
	if (dup2(red->here_fd, red->fd) == -1)
		return (close(fd), perror("dup2"), false);
	execute(red->node);
	if (dup2(fd, red->fd) == -1)
		return (close(fd), perror("dup2"), false);
	close(fd);
	return (true);
}

bool	red_type(t_redirection *red)
{
	int		or;
	char	*tmp2;
	int		fd;

	tmp2 = expand_file(red->file);
	if (!tmp2)
		return (false);
	red->file = tmp2;
	// S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); /* rw-rw-rw- 
	or = open(red->file, red->flags, PREMISSIONS);
	if(or == -1)
		return (perror("open"), false);
	fd = dup(red->fd);
	if (fd == -1)
		return (close(or), perror("dup"), false);
	if (dup2(or, red->fd) == -1)
		return (close(or), close(fd), perror("dup2"), false);
	close(or);
	execute(red->node);
	if (dup2(fd, red->fd) == -1)
		return (close(fd), perror("dup2"), false);
	close(fd);
	return (true);
}

void	_child(t_node *node, int usingp, int closingp, int s)
{
	int	status;

	if (dup2(usingp, s) == -1)
	{
		perror("dup2");
		close(usingp);
		close(closingp);
		exit (1);
	}
	close(usingp);
	close(closingp);
	in_pipe(1);
	execute(node);
	status = exit_status(0, false, false);
	exit(WEXITSTATUS(status));
}

void	wating_for_childs(pid_t *pid)
{
	int	s;

	while (waitpid(pid[0], &s, 0) == -1)
	{
		if (errno != EINTR)
		{
			kill(pid[0], SIGINT);
			perror("waitpid");
			break ;
		}
	}
	while (waitpid(pid[1], &s, 0) == -1)
	{
		if (errno != EINTR) 
		{
			s = 1 << 8;
			kill(pid[1], SIGINT);
			perror("waitpid");
			break ;
		}
	}
	exit_status(s, true, false);
}

bool	pipe_type(t_div *div)
{
	pid_t	pid[2];
	int		p[2];

	if(pipe(p) == -1)
		return (perror("pipe"), false);
	set_signal_handler(SIGINT, SIG_IGN);
	pid[0] = fork();
	if (pid[0] == -1)
		return (close(p[0]), close(p[1]), perror("fork"), false);
	if (pid[0] == 0)
		_child(div->left, p[1], p[0], 1);
	pid[1] = fork();
	if (pid[1] == -1)
	{
		kill(pid[0], SIGINT);
		return (close(p[0]), close(p[1]), perror("fork"), false);
	}
	if (pid[1] == 0)
		_child(div->right, p[0], p[1], 0);
	in_pipe(0);
	close(p[0]);
	close(p[1]);
	wating_for_childs(pid);
	set_signal_handler(SIGINT, sigint_handler);
	return (true);
}

void	and_type(t_div *div)
{
	execute(div->left);
	if (!exit_status(0, false, false))
		execute(div->right);
}

void	or_type(t_div *div)
{
	execute(div->left);
	if (exit_status(0, false, false))
		execute(div->right);
}

void	execute(t_node *node)
{
	bool	b;

	b = true;
	if (node->type == EXEC)
		exec_type(node);
	else if (node->type == HEREDOC)
		b = heredoc_type((t_redirection*)node);
	else if (node->type == RED)
		b = red_type((t_redirection*)node);
	else if (node->type == PIPE)
		b = pipe_type((t_div *)node);
	else if (node->type == AND)
		and_type((t_div*)node);
	else if (node->type == OR)
		or_type((t_div*)node);
	if (!b)
		exit_status(1, true, true);
}

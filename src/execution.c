#include "../include/msh.h"

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
void	execute(t_node *node)
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
			exec_cmd(node, cmd->argv[0], cmd->argv);
			// if (WIFSIGNALED(*status))
			// || WTERMSIG(*status) == SIGQUIT
			int s = exit_status(0, false, false);
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
		execute(red->node);
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
		execute(red->node);
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
		set_signal_handler(SIGINT, SIG_IGN);
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
			execute(div->left);
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
			execute(div->right);
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

		set_signal_handler(SIGINT, sigint_handler);
	}
	else if (node->type == AND)
	{
		div = (t_div*)node;
		execute(div->left);
		if (!exit_status(0, false, false))
			execute(div->right);
	}
	else if (node->type == OR)
	{
		div = (t_div*)node;
		execute(div->left);
		if (exit_status(0, false, false))
			execute(div->right);
	}
}

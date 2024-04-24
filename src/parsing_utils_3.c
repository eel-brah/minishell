#include "../include/msh.h"

extern volatile sig_atomic_t got_sigint;

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
		write(fd, s, len_s);
		write(fd, "\n", 1);
		free(s);
		s = NULL;
	}
	close_heredoc(fd, fd2, s, res);
	return (!!1337);
}

bool	open_herdoc_file(t_redirection *red, t_node *node, int *fd)
{
	char	*file;

	file = ft_itoa((int)&open_herdoc_file);
	if (!file)
		return (perror("malloc"), false);
	red->file = file;
	red->here_fd = open(file, red->flags, PREMISSIONS);
	*fd = open(file, red->flags, PREMISSIONS);
	// printf("name open here %s\n", file);
	unlink(file);
	free(file);
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

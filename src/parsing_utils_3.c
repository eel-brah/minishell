/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:46:47 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/13 18:35:13 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static void	close_heredoc(int fd, int fd2, char *s, char **res)
{
	close(fd);
	close(fd2);
	free(s);
	double_free(res);
	set_signal_handler(SIGINT, sigint_handler);
	got_sigint = 0;
}

static char	**heredoc_setup(char *delim, t_redirection *node,
		unsigned int *len, int *fd2)
{
	char	**res;

	if (!delim)
	{
		print_error_2("minishell", "malloc", strerror(errno));
		return (NULL);
	}
	*fd2 = dup(0);
	if (*fd2 == -1)
	{
		print_error_2("minishell", "dup", strerror(errno));
		return (NULL);
	}
	if (ft_strchr(delim, '\'') || ft_strchr(delim, '"'))
		node->expand = false;
	res = expander(delim, 0, 0, 0);
	free(delim);
	if (!res)
	{
		print_error_2("minishell", "malloc", strerror(errno));
		return (NULL);
	}
	*len = ft_strlen(res[0]);
	return (res);
}

static bool	_reset(int fd, int fd2, char *s, char **res)
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
	unsigned int	len_s;
	t_tmp			tmp;
	int				fd2;

	res = heredoc_setup(delim, node, &tmp.len, &fd2);
	if (!res)
		return (false);
	set_signal_handler(SIGINT, sigint_handler2);
	while (1)
	{
		tmp.env = environ;
		s = readline("> ");
		environ = tmp.env;
		if (got_sigint)
			return (_reset(fd, fd2, s, res));
		len_s = ft_strlen(s);
		if (!s || (tmp.len == len_s && !ft_strncmp(s, res[0], tmp.len)))
			break ;
		(1) && (write(fd, s, len_s), write(fd, "\n", 1));
		(1) && (free(s), s = NULL);
	}
	return (close_heredoc(fd, fd2, s, res), true);
}

bool	open_herdoc_file(t_redirection *red, t_node *node, int *fd)
{
	red->file = name_file_rand((int)&open_herdoc_file);
	if (!red->file)
		return (false);
	red->here_fd = open(red->file, red->flags, PREMISSIONS);
	*fd = open(red->file, red->flags, PREMISSIONS);
	unlink(red->file);
	free(red->file);
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

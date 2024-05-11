/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:57:39 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/11 15:17:36 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static void	signal_exting(int s)
{
	if (WTERMSIG(s) == SIGINT)
	{
		exit_status(130, true, true);
		ft_putchar_fd('\n', 1);
	}
	else if (WTERMSIG(s) == SIGQUIT)
	{
		exit_status(131, true, true);
		reset_term();
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
		signal_exting(GET_STAUS);
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
		fd_read = expand_here_doc(red->here_fd, GET_STAUS, red->expand);
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
	if (or == -1)
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

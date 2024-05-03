/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 20:57:47 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:48:41 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static void	_child(t_node *node, int usingp, int closingp, int s)
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
	status = GET_STAUS;
	exit(WEXITSTATUS(status));
}

static void	wating_for_childs(pid_t *pid)
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

	if (pipe(p) == -1)
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
	if (!GET_STAUS)
		execute(div->right);
}

void	or_type(t_div *div)
{
	execute(div->left);
	if (GET_STAUS)
		execute(div->right);
}

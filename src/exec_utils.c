/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 21:14:50 by eel-brah          #+#    #+#             */
/*   Updated: 2024/06/10 07:21:22 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

int	built_in(t_node *tree, char *prg, char **args)
{
	unsigned int	size;

	size = ft_strlen(prg);
	if (size == ft_strlen("echo")
		&& (!ft_strncmp(prg, "echo", size) || !ft_strncmp(prg, "ECHO", size)))
		return (ft_echo(args + 1));
	else if (size == ft_strlen("pwd")
		&& (!ft_strncmp(prg, "pwd", size) || !ft_strncmp(prg, "PWD", size)))
		return (ft_pwd(args + 1));
	else if (size == ft_strlen("env")
		&& (!ft_strncmp(prg, "env", size) || !ft_strncmp(prg, "ENV", size)))
		return (ft_env(environ, args + 1));
	else if (size == ft_strlen("cd")
		&& !ft_strncmp(prg, "cd", size))
		return (ft_cd(args + 1));
	else if (size == ft_strlen("exit")
		&& !ft_strncmp(prg, "exit", size))
		return (ft_exit(tree, args + 1));
	else if (size == ft_strlen("export")
		&& !ft_strncmp(prg, "export", size))
		return (ft_export(args + 1));
	else if (size == ft_strlen("unset")
		&& !ft_strncmp(prg, "unset", size))
		return (ft_unset(environ, args + 1));
	return (-1);
}

static void	execute_prg_child(char *prg, char **args, char **eenv)
{
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_DFL);
	execve(prg, args, eenv);
	print_error_2("minishell", "execve", strerror(errno));
	exit(1);
}

static void	wating_for_child(pid_t pid)
{
	int		s;

	while (waitpid(pid, &s, 0) == -1)
	{
		if (errno != EINTR)
		{
			s = 1 << 8;
			print_error_2("minishell", "waitpid", strerror(errno));
			kill(pid, SIGINT);
			break ;
		}
	}
	exit_status(s, true, false);
}

void	execute_prg(char *prg, char **args)
{
	char	**eenv;
	pid_t	pid;

	set_signal_handler(SIGINT, SIG_IGN);
	ft_setenv(environ, "_=", prg);
	eenv = env_without_empty(environ);
	if (!eenv)
	{
		print_error_2("minishell", "malloc", strerror(errno));
		exit_status(1, true, true);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		print_error_2("minishell", "fork", strerror(errno));
		exit_status(1, true, true);
		free(eenv);
		return ;
	}
	if (pid == 0)
		execute_prg_child(prg, args, eenv);
	wating_for_child(pid);
	free(eenv);
	set_signal_handler(SIGINT, sigint_handler);
}

void	prg_with_path(char *prg, char **args)
{
	struct stat	statbuf;

	if (stat(prg, &statbuf))
	{
		print_error_2("minishell", prg, strerror(errno));
		if (errno == 20)
			exit_status(126, true, true);
		else
			exit_status(127, true, true);
		return ;
	}
	errno = 0;
	if (access(prg, X_OK) == -1 || S_ISDIR(statbuf.st_mode))
	{
		if (!errno)
			errno = 21;
		print_error_2("minishell", prg, strerror(errno));
		exit_status(126, true, true);
		return ;
	}
	execute_prg(prg, args);
}

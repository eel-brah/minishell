#include "../include/msh.h"

extern char **environ;

int	built_in(t_node *tree, char *prg, char **args)
{
	unsigned int size;

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

char	*ft_getenv(char **env, char *s)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(s);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], s, len) == 0 && env[i][len] == '=')
			return ((char *)(&env[i][len + 1]));
		i++;
	}
	return (NULL);
}

void	execute_prg_child(char *prg, char **args, char **eenv)
{
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_DFL);
	execve(prg, args, eenv);
	print_error_2("minishell", "execve", strerror(errno));
	exit(1);
}

void	wating_for_child(pid_t pid)
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

void	prg_has_path(char *prg, char **args)
{
	struct stat statbuf;

	if (stat(prg, &statbuf))
	{
		print_error_2("minishell", prg, strerror(errno));
		exit_status(127, true, true);
		return ;
	}
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

void	exec_cmd(t_node *tree, char *prg, char **args)
{
	char	*path;
	char	**paths;
	int		i;
	char	*slashed;
	char	*full_path;
	char	*pr_denied;
	int		t;

	t = built_in(tree, prg, args);
	if (t != -1)
		exit_status(t, true, true);
	else if (ft_strchr(prg, '/'))
		prg_has_path(prg, args);
	else
	{
		path = getenv("PATH");
		if (path && *prg && !(!ft_strncmp(prg, ".", 2) || !ft_strncmp(prg, "..", 3)))
		{
			paths = ft_split(path, ':');
			if (!paths)
			{
				print_error_2("minishell", "malloc", strerror(errno));
				exit_status(1, true, true);
				return ;
			}
			slashed = ft_strjoin("/", prg);
			if (!slashed)
			{
				print_error_2("minishell", "malloc", strerror(errno));
				double_free(paths);
				exit_status(1, true, true);
				return ;
			}
			i = 0;
			pr_denied = NULL;
			while (paths[i])
			{
				full_path = ft_strjoin(paths[i], slashed);
				if (!full_path)
				{
					print_error_2("minishell", "malloc", strerror(errno));
					double_free(paths);
					free(slashed);
					exit_status(1, true, true);
					return ;
				}
				if (access(full_path, F_OK) == 0)
				{
					if (access(full_path, X_OK) == -1)
					{
						if (!pr_denied)
							pr_denied = full_path;
						i++;
						continue;
					}
					break ;
				}
				free(full_path);
				i++;
			}
			bool	end = !!paths[i];
			free(slashed);
			double_free(paths);
			if (!end && pr_denied)
			{
				print_error_2("minishell", pr_denied, "Permission denied");
				exit_status(126, true, true);
				free(pr_denied);
				return ;
			}
			else if (!end)
			{
				print_error_2("minishell", prg, "command not found");
				exit_status(127, true, true);
				return ;
			}
			else
			{
				free(pr_denied);
				execute_prg(full_path, args);
				free(full_path);
			}
		}
		else
		{
			print_error_2("minishell", prg, "command not found");
			exit_status(127, true, true);
			return ;
		}
	}
	return ;
}

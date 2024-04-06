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

void	exec_cmd(t_node *tree, char *prg, char **args)
{
	char	*path;
	char	**paths;
	int		i;
	char	*slashed;
	char	*full_path;
	pid_t	pid;
	struct stat statbuf;
	bool	is_path;
	char	*pr_denied;
	bool	acc;
	int t;
		int s;


	if (!prg)
	{
		exit_status(0, true, true);
		return ;
	}
	t = built_in(tree, prg, args);
	if (t != -1)
	{
		exit_status(t, true, true);
		return ;
	}
	if (!ft_strncmp(prg, ".", 2) || !ft_strncmp(prg, "..", 3))
	{
		print_error(prg, "command not found");
		exit_status(127, true, true);
		return ;
	}
	is_path = !!ft_strchr(prg, '/');
	acc = access(prg, F_OK);
	if (is_path && acc)
	{
		perror(prg);
		exit_status(127, true, true);
		return ;
	}
	else if (is_path && !acc)
	{
		if (access(prg, X_OK))
		{
			perror(prg);
			exit_status(126, true, true);
			return ;
		}
		if (stat(prg, &statbuf))
		{
			perror("stat");
			exit_status(1, true, true);
			return ;
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			print_error(prg, "Is a directory");
			exit_status(126, true, true);
			return ;
		}

		// signal(SIGINT, SIG_IGN);
		set_signal_handler(SIGINT, SIG_IGN);
		char **eenv = env_without_empty(environ);
		// if NULL
		pid = fork();
		if (pid == 0)
		{
			set_signal_handler(SIGINT, SIG_DFL);
			set_signal_handler(SIGQUIT, SIG_DFL);
			// ft_change_last_pro(&env, args);
			execve(prg, args, eenv);
			// handle signals if faild
			free(eenv);
			perror("execve");
			exit_status(1, true, true);
			return ;
		}
		while (waitpid(pid, &s, 0) == -1)
		{
			if (errno != EINTR)
			{
				free(eenv);
				exit_status(1, true, true);
				return ;
			}
		}
		free(eenv);
		set_signal_handler(SIGINT, sigint_handler);
		set_signal_handler(SIGQUIT, SIG_IGN);
	}
	else
	{
		path = getenv("PATH");
		if (path && *prg)
		{
			paths = ft_split(path, ':');
			if (!paths)
			{
				perror("malloc");
				exit_status(1, true, true);
				return ;
			}
			slashed = ft_strjoin("/", prg);
			if (!slashed)
			{
				perror("malloc");
				double_free(paths);
				exit_status(1, true, true);
				return ;
			}
			i = 0;
			pr_denied = NULL;
			while (paths[i])
			{
				full_path = ft_strjoin(paths[i], slashed);
				if (access(full_path, F_OK) == 0)
				{
					if (access(full_path, X_OK))
					{
						if (!pr_denied)
							pr_denied = ft_strdup(full_path);
						i++;
						continue;
					}
					break ;
				}
				free(full_path);
				i++;
			}
			if (!paths[i] && pr_denied)
			{
				print_error(pr_denied, "Permission denied");
				exit_status(126, true, true);
				return ;
			}
			else if (!paths[i])
			{
				print_error(prg, "command not found");
				exit_status(127, true, true);
				return ;
			}
			else
			{
				set_signal_handler(SIGINT, SIG_IGN);
				// signal(SIGINT, SIG_IGN);
				char **eenv = env_without_empty(environ);
				pid = fork();
				if (pid == 0)
				{
					set_signal_handler(SIGINT, SIG_DFL);
					set_signal_handler(SIGQUIT, SIG_DFL);
					// ft_change_last_pro(&env, args);
					execve(full_path, args, eenv);
					perror("execve");
					free(eenv);
					exit_status(1, true, true);
					return ;
				}
				while (waitpid(pid, &s, 0) == -1)
				{
					if (errno != EINTR) 
					{
						free(eenv);
						exit_status(1, true, true);
						return ;
					}
				}
				free(eenv);
				set_signal_handler(SIGINT, sigint_handler);
				set_signal_handler(SIGQUIT, SIG_IGN);
				free(full_path);
			}
			free(pr_denied);
			double_free(paths);
			free(slashed);
		}
		else
		{
			print_error(prg, "command not found");
			exit_status(127, true, true);
			return ;
		}
	}
	exit_status(s, true, false);
	return ;
}

// int main()
// {
// 	char	*line;
// 	char	*cmd;
// 	char	**args;

// 	while (true)
// 	{
// 		line = readline("$");
// 		if (line && *line)
// 		{
// 			cmd = ft_strtrim(line, "\t\n\v\f\r ");
// 			if (!cmd)
// 				perror("malloc");
// 			else if (*cmd)
// 			{
// 				args = ft_split(cmd, ' ');
// 				if (!args)
// 				{
// 					perror("malloc");
// 					free(cmd);
// 					continue;
// 				}
// 				exec_cmd(args[0], args);
// 				double_free(args);
// 			}
// 			free(cmd);
// 		}
// 		free(line);
// 	}
// }
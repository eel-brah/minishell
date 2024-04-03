#include "../include/msh.h"

extern char **environ;

int	built_in(t_node *tree, int status, char *prg, char **args, char ***env)
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
		return (ft_env(*env, args + 1));
	else if (size == ft_strlen("cd") 
		&& !ft_strncmp(prg, "cd", size))
		return (ft_cd(args + 1));
	else if (size == ft_strlen("exit") 
		&& !ft_strncmp(prg, "exit", size))
		return (ft_exit(tree, args + 1, *env, status));
	else if (size == ft_strlen("export") 
		&& !ft_strncmp(prg, "export", size))
		{
			bool a = ft_export(env, args + 1);
			environ = *env;
			return (a);
		}
	else if (size == ft_strlen("unset") 
		&& !ft_strncmp(prg, "unset", size))
		return (ft_unset(*env, args + 1));
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

void	sig_hand1(int sig)
{
	(void) sig;
	write(1, "\n", 1);
}

int	exec_cmd(t_node *tree, int status, char *prg, char **args, char ***env)
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

	if (!prg)
		return (0);
	t = built_in(tree, status, prg, args, env);
	if (t != -1)
		return (t << 8);
	if (!ft_strncmp(prg, ".", 2) || !ft_strncmp(prg, "..", 3))
	{
		print_error(prg, "command not found");
		return(127 << 8);
	}
	is_path = !!ft_strchr(prg, '/');
	acc = access(prg, F_OK);
	if (is_path && acc)
	{
		perror(prg);
		return(127 << 8);
	}
	else if (is_path && !acc)
	{
		if (access(prg, X_OK))
		{
			perror(prg);
			return(126 << 8);
		}
		if (stat(prg, &statbuf))
		{
			perror("stat");
			return(1 << 8);
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			print_error(prg, "Is a directory");
			return(126 << 8);
		}
		// pid = fork();
		// if (pid == -1)
		// {
		// 	perror("fork");
		// 	return(1);
		// }
		// if (pid == 0)
		// {
		// 	execve(prg, args, env);
		// 	perror("execve");
		// 	return(1);
		// }
		// waitpid(pid, &status, 0);

		signal(SIGINT, SIG_IGN);
		char **eenv = env_without_empty(*env);
		// if NULL
		pid = fork();
		if (pid == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			execve(prg, args, eenv);
			// handle signals if faild
			perror("execve");
			return(1 << 8);
		}
		while (waitpid(pid, &status, 0) == -1)
		{
			if (errno != EINTR) 
				return (1 << 8);
		}
		free(eenv);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sigint_handler);
	}
	else
	{
		path = ft_getenv(*env, "PATH");
		if (path && *prg)
		{
			paths = ft_split(path, ':');
			if (!paths)
			{
				perror("malloc");
				return(1 << 8);
			}
			slashed = ft_strjoin("/", prg);
			if (!slashed)
			{
				perror("malloc");
				double_free(paths);
				return(1 << 8);
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
				return(126 << 8);
			}
			else if (!paths[i])
			{
				print_error(prg, "command not found");
				return(127 << 8);
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				char **eenv = env_without_empty(*env);
				pid = fork();
				if (pid == 0)
				{
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					execve(full_path, args, eenv);
					perror("execve");
					return(1 << 8);
				}
				while (waitpid(pid, &status, 0) == -1)
				{
					if (errno != EINTR) 
						return (1 << 8);
				}
				free(eenv);
				signal(SIGQUIT, SIG_IGN);
				signal(SIGINT, sigint_handler);
				free(full_path);
			}
			free(pr_denied);
			double_free(paths);
			free(slashed);
		}
		else
		{
			print_error(prg, "command not found");
			return(127 << 8);
		}
	}
	return(status);
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
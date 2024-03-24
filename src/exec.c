#include "../include/msh.h"

extern char **environ;

bool	built_in(char *prg, char **args, char **env)
{
	unsigned int size;

	size = ft_strlen(prg);
	if (size == ft_strlen("echo") 
		&& !ft_strncmp(prg, "echo", size))
	{
		ft_echo(args + 1);
		return (1);
	}
	else if (size == ft_strlen("pwd") 
		&& !ft_strncmp(prg, "pwd", size))
	{
		ft_pwd(args + 1);
		return (1);
	}
	else if (size == ft_strlen("env") 
		&& !ft_strncmp(prg, "env", size))
	{
		ft_env(env, args + 1);
		return (1);
	}
	return (0);
}

void	exec_cmd(char *prg, char **args, char **env)
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

	if (built_in(prg, args, env))
		exit(0);
	is_path = !!ft_strchr(prg, '/');
	acc = access(prg, F_OK);
	if (is_path && acc)
	{
		perror(prg);
		exit(1);
	}
	else if (is_path && !acc)
	{
		if (access(prg, X_OK))
		{
			perror(prg);
			exit(1);
		}
		if (stat(prg, &statbuf))
		{
			perror("stat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			ft_putstr_fd(prg, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			execve(prg, args, env);
			perror("execve");
			exit(1);
		}
		waitpid(pid, NULL, 0);
	}
	else
	{
		path = getenv("PATH");
		if (path)
		{
			paths = ft_split(path, ':');
			if (!paths)
			{
				perror("malloc");
				exit(1);
			}
			slashed = ft_strjoin("/", prg);
			if (!slashed)
			{
				perror("malloc");
				double_free(paths);
				exit(1);
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
				ft_putstr_fd(pr_denied, 2);
				ft_putstr_fd(": Permission denied\n", 2);
			}
			else if (!paths[i])
			{
				ft_putstr_fd(prg, 2);
				ft_putstr_fd(": command not found\n", 2);
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					execve(full_path, args, env);
					perror("execve");
				}
				else
					waitpid(pid, NULL, 0);
				free(full_path);
			}
			free(pr_denied);
			double_free(paths);
			free(slashed);
		}
		else
		{
			ft_putstr_fd(prg, 2);
			ft_putstr_fd(": command not found\n", 2);
		}
	}
	exit(0);
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
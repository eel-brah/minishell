#include "../include/msh.h"

extern char **environ;

int	built_in(t_node *tree, int r, char *prg, char **args, char **env)
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
		return (ft_env(env, args + 1));
	else if (size == ft_strlen("cd") 
		&& !ft_strncmp(prg, "cd", size))
		return (ft_cd(args + 1));
	else if (size == ft_strlen("exit") 
		&& !ft_strncmp(prg, "exit", size))
		return (ft_exit(tree, args + 1, env, r));
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
int	exec_cmd(char *prg, char **args, char **env)
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
	int		r = 0;
	
	is_path = !!ft_strchr(prg, '/');
	acc = access(prg, F_OK);
	if (is_path && acc)
	{
		perror(prg);
		return(127);
	}
	else if (is_path && !acc)
	{
		if (access(prg, X_OK))
		{
			perror(prg);
			return(126);
		}
		if (stat(prg, &statbuf))
		{
			perror("stat");
			return(1);
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			print_error(prg, "Is a directory");
			return(126);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return(1);
		}
		if (pid == 0)
		{
			execve(prg, args, env);
			perror("execve");
			return(1);
		}
		waitpid(pid, &r, 0);
	}
	else
	{
		path = ft_getenv(env, "PATH");
		if (path && *prg)
		{
			paths = ft_split(path, ':');
			if (!paths)
			{
				perror("malloc");
				return(1);
			}
			slashed = ft_strjoin("/", prg);
			if (!slashed)
			{
				perror("malloc");
				double_free(paths);
				return(1);
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
				return(126);
			}
			else if (!paths[i])
			{
				print_error(prg, "command not found");
				return(127);
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					execve(full_path, args, env);
					perror("execve");
					return(1);
				}
				waitpid(pid, &r, 0);
				free(full_path);
			}
			free(pr_denied);
			double_free(paths);
			free(slashed);
		}
		else
		{
			print_error(prg, "command not found");
			return(127);
		}
	}
	return(r >> 8);
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
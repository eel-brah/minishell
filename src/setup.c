#include "../include/msh.h"

extern int rl_catch_signals;
extern volatile sig_atomic_t got_sigint;

char	**setup(int argc, char **argv, char **env)
{
	char	**_env;

	(void)argv;
	if (argc > 1)
	{
		print_error("minishell", "Invalid number of arguments");
		return (NULL);
	}
	set_signal_handler(SIGINT, sigint_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
	_env = create_env(env);
	if (!_env)
		return (NULL);
	rl_catch_signals = 0;
	environ = _env;
	return (_env);
}

void sigint_handler(int sig)
{
	exit_status(1, true, true);
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void sigint_handler2(int sig)
{
	exit_status(1, true, true);
	got_sigint = 1;
	close(0);
	(void)sig;
}

void	set_signal_handler(int signal, void (*fun)(int))
{
	struct sigaction	sig;

	sig.sa_handler = fun;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(signal, &sig, NULL);
}
char	*handl_path_pwd(char **env, size_t *i, char **ptr)
{
	if (env_is_there(env, "PATH") == false)
	{
		printf("herererer\n");
		ptr[(*i)++] = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
		if (!ptr[(*i) - 1])
		{
			perror("malloc");
			double_free(ptr);
			return (NULL);
		}
	}
	if (env_is_there(env, "PWD") == false)
	{
		printf("herererer\n");
		char *tmp = getcwd(NULL, 0);
		if (!tmp)
			return (perror("getcwd"), NULL);
		char *tmp1 = ft_strjoin("PWD=", tmp);
		ptr[(*i)++] = tmp1;
		if (!ptr[(*i) - 1])
		{
			perror("malloc");
			double_free(ptr);
			return (NULL);
		}
	}
	return ((char *)1337);
}
char	**create_env(char **env)
{
	size_t	size;
	char	**ptr;
	size_t	i;
	int		add;

	add = 0;
	size = count_args(env);
	if (size == 0)
		return (create_new_env());
	if (env_is_there(env, "PATH") == false)
		add++;
	if (env_is_there(env, "PWD") == false)
		add++;
	ptr = malloc((size + 1 + add) * sizeof(env));
	if (!ptr)
		return (perror("malloc"), NULL);
	i = 0;
	while (env[i])
	{
		ptr[i] = ft_strdup(env[i]);
		if (!ptr[i])
		{
			perror("malloc");
			double_free(ptr);
			return (NULL);
		}
		i++;
	}
	handl_path_pwd(env, &i, ptr);
	ptr[i] = NULL;
	return (ptr);
}

char	**create_new_env()
{
	char	**env;
	char	*tmp;
	char	*tmp1;

	printf("here pWd\n");
	env = malloc(sizeof(char *) * 5);
	if (!env)
		return (perror("malloc"), NULL);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), NULL);
	tmp1 = ft_strjoin("PWD=", tmp);
	env[0] = tmp1;
	tmp = ft_strdup("SHLVL=1");
	if (!tmp || !tmp1)
		return (perror("malloc"), NULL);
	env[1]= tmp;
	tmp = ft_strdup("_=/usr/bin/minishell"); // handle it
	if (!tmp)
		return (perror("malloc"), NULL);
	env[2]= tmp;
	tmp = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	if (!tmp)
		return (perror("malloc"), NULL); // free the previous ones
	env[3]= tmp;
	return (env[4]= NULL, env);
}

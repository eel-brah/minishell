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
	return (environ);
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
bool	handl_path_pwd(char **env, size_t *i, char **ptr)
{
	if (env_is_there(env, "PATH") == false)
	{
		ptr[(*i)++] = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
		if (!ptr[(*i) - 1])
			return (perror("malloc"), double_free(ptr), false);
	}
	if (env_is_there(env, "SHLVL") == false)
	{
		ptr[(*i)++] = ft_strdup("SHLVL=1");
		// printf("%s\n",ptr[(*i) - 1]);
		if (!ptr[(*i) - 1])
			return (perror("malloc"), double_free(ptr), false);
	}
	if (env_is_there(env, "PWD") == false)
	{
		char *tmp = getcwd(NULL, 0);
		if (!tmp)
			return (perror("getcwd"), false);
		char *tmp1 = ft_strjoin("PWD=", tmp);
		free(tmp);
		ptr[(*i)++] = tmp1;
		if (!ptr[(*i) - 1])
			return (perror("malloc"), double_free(ptr), false);
	}
	return (!!1337);
}
void	check_path_pwd(char **env, size_t *i, int *add)
{
	*add = 0;
	*i = 0;
	if (env_is_there(env, "PATH") == false)
		(*add)++;
	if (env_is_there(env, "PWD") == false)
		(*add)++;
	if (env_is_there(env, "SHLVL") == false)
		(*add)++;
}
bool	is_all_digit(char *s)
{
	while (s && *s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}
bool	handle_shlvl(char *val, char **ptr, int i)
{
	int		shlvl;
	char	**spl;
	int		c;
	char	*tmp;

	spl = ft_split(val, '=');
	if (!spl)
		return (perror("malloc"), false);
	c = count_args(spl);
	if ( c > 2 || (c == 2 && !is_all_digit(spl[1])) || c == 1)
		ptr[i] = ft_strdup("SHLVL=1");
	else if (c == 2)
	{
		shlvl = ft_atoi(spl[1]) + 1;
		if (shlvl == 1000)
			ptr[i] = ft_strdup("SHLVL=");
		else if (shlvl > 1000)
		{
			print_error("minishell", "warning: shell level is too high, resetting to 1");
			ptr[i] = ft_strdup("SHLVL=1");
		}
		else
		{
			tmp = ft_itoa(shlvl);
			if (!tmp)
				return (perror("malloc"), d_free(spl), false);
			ptr[i] = ft_strjoin("SHLVL=", tmp);
			free(tmp);
		}
	}
	double_free(spl);
	if (!ptr[i])
		return (perror("malloc"), false);
	return (true);
}
char	**create_env(char **env)
{
	size_t	size;
	char	**ptr;
	size_t	i;
	int		add;

	size = count_args(env);
	if (size == 0)
		return (create_new_env());
	check_path_pwd(env, &i, &add);
	ptr = malloc((size + 1 + add) * sizeof(env));
	// printf("%p\n",ptr);
	if (!ptr)
		return (perror("malloc"), NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			if (!handle_shlvl(env[i], ptr, i))
				return (perror("malloc"), double_free(ptr), NULL);
		}
		else
			ptr[i] = ft_strdup(env[i]);
		if (!ptr[i])
			return (perror("malloc"), double_free(ptr), NULL);
		i++;
	}
	if (!handl_path_pwd(env, &i, ptr))
		return (NULL);
	return (ptr[i] = NULL, ptr);
}

char	**create_new_env()
{
	char	**env;
	char	*tmp;
	char	*tmp1;

	env = malloc(sizeof(char *) * 5);
	if (!env)
		return (perror("malloc"), NULL);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), NULL);
	tmp1 = ft_strjoin("PWD=", tmp);
	free(tmp);
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

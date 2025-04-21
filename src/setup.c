/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 21:25:42 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/13 21:38:47 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

volatile sig_atomic_t	g_got_sigint = 0;

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
	_env = create_env(env, 0, 0, 0);
	if (!_env)
		return (NULL);
	rl_catch_signals = 0;
	environ = _env;
	return (environ);
}

void	sigint_handler(int sig)
{
	exit_status(1, true, true);
	(void)sig;
	ssize_t ret = write(1, "\n", 1);
  if (ret == -1)
    perror("write failed");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler2(int sig)
{
	exit_status(1, true, true);
	g_got_sigint = sig;
	close(0);
}

void	set_signal_handler(int signal, void (*fun)(int))
{
	struct sigaction	sig;

	sig.sa_handler = fun;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	if (sigaction(signal, &sig, NULL) == -1)
		print_error_2("minishell", "sigaction", strerror(errno));
}

bool	handl_path_pwd(char **env, size_t *i, char **p)
{
	char	*tmp;
	char	*tmp1;

	if (env_is_there(env, "SHLVL") == false)
	{
		p[(*i)++] = ft_strdup("SHLVL=1");
		if (!p[(*i) - 1])
			return (perror("malloc"), d_free(p), false);
	}
	if (env_is_there(env, "PATH") == false)
	{
		p[*i] = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
		if (((*i)++ || *i != 1) && !p[(*i) - 1])
			return (perror("malloc"), d_free(p), false);
	}
	if (env_is_there(env, "PWD") == false)
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			return (perror("getcwd"), false);
		(1) && (tmp1 = ft_strjoin("PWD=", tmp), free(tmp), p[(*i)++] = tmp1);
		if (!p[(*i) - 1])
			return (perror("malloc"), d_free(p), false);
	}
	return (!!1337);
}

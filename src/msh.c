/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:41 by eel-brah          #+#    #+#             */
/*   Updated: 2024/04/27 20:16:14 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

volatile sig_atomic_t got_sigint = 0;
extern char **environ;

void fu()
{
	system("leaks minishell");
}
void	ft_printenv_no_empty1(char **env)
{
	int fd = open("file1", O_CREAT | O_RDWR, 0777);
	while (env && *env)
	{
		ft_putendl_fd(*env, fd);
		env++;
	}
}

void	ft_printenv_no_empty2(char **env)
{
	int fd = open("file2", O_CREAT | O_RDWR, 0777);
	while (env && *env)
	{
		ft_putendl_fd(*env, fd);
		env++;
	}
}

void	fix_env(char **env)
{
	char		*columns;
	char		*lines;
	static bool	flag;
	
	if (!flag)
	{
		flag = true;
		columns = getenv("COLUMNS");
		lines = getenv("LINES");
		environ = env;
		if (lines )
			ft_setenv(environ, "LINES=", lines);
		if (columns)
			ft_setenv(environ, "COLUMNS=", columns);
	}
	else
		environ = env;
}

char	*get_cmd(char *prompt)
{
	char	*cmd;
	char	**env;

	env = environ;
	if (!prompt)
		cmd = readline("$ ");
	else
		cmd = readline(prompt);
	fix_env(env);
	// environ = env;
	free(prompt);
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}

void	pre_trv(t_node *node) 
{
	if (node->type == EXEC || node->type == RED)
	{
		t_node *ptr = node;
		while (ptr->type == RED)
		{
			printf("RED %s\n", ((t_redirection *)ptr)->file);
			ptr = ((t_redirection *)ptr)->node;
		}
		printf("EXEX: %s\n", ((t_exec *)ptr)->argv[0]);
		return ;
	}
	else
		printf("%i\n", node->type);
	pre_trv(((t_div *)node)->left);
	pre_trv(((t_div *)node)->right);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd;
	t_node	*tree;

	if (!setup(argc, argv, env))
		exit(1);
	while (1)
	{
		cmd = get_cmd(get_prompt());
		if (!cmd)
			ft_exit(NULL, NULL);
		else if (!*cmd)
		{
			free(cmd);
			continue;
		}
		tree = parse_cmd(cmd);
		free(cmd);
		if (!tree)
			continue;
		execute(tree);
		free_cmdtree(tree);
	}
}

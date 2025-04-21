/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 21:14:14 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:56:36 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static bool	create_paths_and_slashed(char *prg, char *path,
	char ***paths, char **slashed)
{
	*paths = ft_split(path, ':');
	if (!*paths)
	{
		print_error_2("minishell", "malloc", strerror(errno));
		exit_status(1, true, true);
		return (false);
	}
	*slashed = ft_strjoin("/", prg);
	if (!*slashed)
	{
		print_error_2("minishell", "malloc", strerror(errno));
		double_free(*paths);
		exit_status(1, true, true);
		return (false);
	}
	return (true);
}

static char	*look_for_prg(char *slashed,
		char **paths, char **pr_denied, bool *end)
{
	char		*full_path = NULL;
	struct stat	statbuf;

	while (*paths)
	{
		full_path = ft_strjoin(*paths, slashed);
		if (!full_path)
			return (NULL);
		if (stat(full_path, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode))
		{
			if (access(full_path, X_OK) == -1)
			{
				if (!*pr_denied)
					*pr_denied = full_path;
				paths++;
				continue ;
			}
			break ;
		}
		free(full_path);
		paths++;
	}
	*end = !!*paths;
	return (full_path);
}

static bool	prg_final_check(bool end, char *pr_denied, char *prg)
{
	if (!end && pr_denied)
	{
		print_error_2("minishell", pr_denied, "Permission denied");
		exit_status(126, true, true);
		free(pr_denied);
		return (false);
	}
	else if (!end)
	{
		print_error_2("minishell", prg, "command not found");
		exit_status(127, true, true);
		return (false);
	}
	return (true);
}

static void	prg_with_no_path(char *prg, char **args, char *path)
{
	char	**paths;
	char	*slashed;
	char	*pr_denied;
	char	*full_path;
	bool	end;

	if (!create_paths_and_slashed(prg, path, &paths, &slashed))
		return ;
	pr_denied = NULL;
	full_path = look_for_prg(slashed, paths, &pr_denied, &end);
	free(slashed);
	double_free(paths);
	if (!full_path)
	{
		print_error_2("minishell", "malloc", strerror(errno));
		exit_status(1, true, true);
		return ;
	}
	if (!prg_final_check(end, pr_denied, prg))
		return ;
	free(pr_denied);
	execute_prg(full_path, args);
	free(full_path);
}

void	exec_cmd(t_node *tree, char *prg, char **args)
{
	char	*path;
	int		t;

	if (!prg)
		return ;
	t = built_in(tree, prg, args);
	if (t != -1)
		exit_status(t, true, true);
	else if (ft_strchr(prg, '/'))
		prg_with_path(prg, args);
	else
	{
		path = getenv("PATH");
		if (path && *prg && !(!ft_strncmp(prg, ".", 2)
				|| !ft_strncmp(prg, "..", 3)))
			prg_with_no_path(prg, args, path);
		else
		{
			print_error_2("minishell", prg, "command not found");
			exit_status(127, true, true);
			return ;
		}
	}
	return ;
}

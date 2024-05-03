/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:43:17 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 19:43:17 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static void	get_evnr(char **user, char **cwd, int *len)
{
	char	**ptr;
	char	**t_ptr;

	*user = getenv("USER");
	*cwd = getenv("PWD");
	*len = ft_strlen(*cwd);
	if (*len > 1)
	{
		ptr = ft_split(*cwd, '/');
		if (ptr)
		{
			t_ptr = ptr;
			while (*ptr)
				*cwd = *ptr++;
			*cwd = ft_strdup(*cwd);
			double_free(t_ptr);
		}
	}
}

static char	*create_prompt(char *user, char *cwd)
{
	char	*prompt;
	char	*tmp[5];
	int		i;
	int		j;

	prompt = NULL;
	i = 0;
	if (user)
	{
		prompt = ft_strjoin(prompt, user);
		tmp[i++] = prompt;
		if (cwd)
		{
			prompt = ft_strjoin(prompt, " ");
			tmp[i++] = prompt;
			prompt = ft_strjoin(prompt, cwd);
			tmp[i++] = prompt;
		}
	}
	prompt = ft_strjoin(prompt, "$ ");
	j = 0;
	while (j < i)
		free(tmp[j++]);
	return (prompt);
}

char	*get_prompt(void)
{
	char	*user;
	char	*cwd;
	char	*prompt;
	int		len;

	prompt = getenv("PROMPT");
	if (prompt)
		return (ft_strdup(prompt));
	get_evnr(&user, &cwd, &len);
	prompt = create_prompt(user, cwd);
	if (len > 1)
		free(cwd);
	return (prompt);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:18:26 by eel-brah          #+#    #+#             */
/*   Updated: 2024/03/22 08:18:26 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_H
# define MSH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <fcntl.h>

#include <string.h>
#include "../libft/include/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>


typedef struct s_node 
{
	int	type;
}	t_node;

typedef struct s_exec
{
	int		type;
	char	**argv;
	char	**eargv;
}	t_exec;

typedef struct s_div
{
	int		type;
	t_node	*left;
	t_node	*right;
}	t_div;

typedef struct s_redirection
{
	int		type;
	t_node	*node;
	int		fd;
	char	*file;
	char	*efile;
	int		flags;
}	t_redirection;

#define WHITESPACES "\t\n\v\f\r "
#define SYMBOL "<>|&()"

#define WORD 'w'
#define PIPELINE 'p'
#define OAND 'a'
#define OOR 'o'
#define OUTPUT_REDIRECTION 'r'
#define INPUT_REDIRECTION 'R'
#define APPEND_REDIRECTION 'd'
#define HEREDOC 'h'
#define OPEN_PER 't'
#define CLOSE_PER 'T'

#define PREMISSIONS 0644

#define EXEC 1
#define PIPE 2
#define RED  3
#define AND  4
#define OR   5


size_t	count_args(char **ptrs);
void	free_cmdtree(t_node *tree);
int		ft_echo(char **args);
int		ft_env(char **env, char **args);
int		ft_pwd(char **args);
// int		ft_unset(char **env, char **args);
int		ft_cd(char **args);
int		built_in(char *prg, char **args, char **env);

void	ft_printenv(char **env);
char	**ptrs_realloc(char **tokens, char *arg);
void	double_free(char **ptrs);
void	print_error(char *source, char *error);
void	print_error_2(char *source, char *arg,char *error);
bool	is_valid_variable_name(char *s);

void	exec_cmd(char *prg, char **args, char **env);
char	*get_prompt();

#endif
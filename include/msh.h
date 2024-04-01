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
#include <limits.h>
#include <signal.h>

#include <string.h>
#include "../libft/include/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

typedef struct s_node 
{
	int	type;
}	t_node;

typedef struct s_exec
{
	int		type;
	char	**argv;
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
	int		flags;
}	t_redirection;

#define WHITESPACES "\t\n\v\f\r "
#define SYMBOL "<>|&()"

#define WORD 'w'
#define PIPELINE 'p'
#define OAND 'a'
#define OOR 'o'
#define OUT_RED 'r'
#define IN_RED 'R'
#define APP_RED 'd'
#define HEREDOC 'h'
#define OPEN_PER 't'
#define CLOSE_PER 'T'
#define ERROR 'e'

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
int		ft_unset(char **env, char **args);
int		ft_exit(t_node *tree, char **args, char **env, int r);
int		ft_cd(char **args);
int		built_in(t_node *tree, int r, char *prg, char **args, char ***env);
char	*strdup_v2(char *start, char *end);
ssize_t	ft_atoi_v2(char *s, bool *valid);
bool	ft_export(char ***env, char **args);

void	ft_printenv(char **env);
char	**ptrs_realloc(char **tokens, char *arg);
void	double_free(char **ptrs);
void	print_error(char *source, char *error);
void	print_error_2(char *source, char *arg,char *error);
bool	is_valid_variable_name(char *s);

int		exec_cmd(char *prg, char **args, char **env);
char	*get_prompt();

void	sigint_handler(int sig);

char	**expander(char *s, int here_doc, int expand);
#endif
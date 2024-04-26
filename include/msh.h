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
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include "get_next_line.h"
// #define malloc(x) NULL
#include <string.h>
#include "../libft/include/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

typedef struct s_elem
{
	int		wild;
	int		index;
	int		here_doc;
	int		capacity;
	int		qoute;
	int		expand;
	int		q;
	int		i;
	char	*arr;
	char	*tmp;
}	t_elem;

typedef struct s_node 
{
	int	type;
}	t_node;

typedef struct s_fullenv
{
	char **env;
	char **extra_env;
}	t_fullenv;

typedef struct s_exec
{
	int		type;
	char	**argv;
}	t_exec;

typedef struct wild_patt
{
	char	*wild;
	char	*tmp;
	int		quote;
	int		q;
}	t_wild_patt;

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
	int		here_fd;
	bool	expand;
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
#define HERE_DOC 'h'
#define OPEN_PER 't'
#define CLOSE_PER 'T'
#define ERROR 'e'

#define REDIRECTION token == OUT_RED || token == IN_RED || token == APP_RED

#define PREMISSIONS 0644

#define EXEC	1
#define PIPE	2
#define RED		3
#define AND		4
#define OR		5
#define HEREDOC 6

#define GET_STAUS exit_status(0, false)

size_t	count_args(char **ptrs);
void	free_cmdtree(t_node *tree);
int		ft_echo(char **args);
int		ft_env(char **env, char **args);
int		ft_pwd(char **args);
int		ft_unset(char **env, char **args);
int		ft_exit(t_node *tree, char **args);
int		ft_cd(char **args);
int		built_in(t_node *tree, char *prg, char **args);
char	*strdup_v2(char *start, char *end);
ssize_t	ft_atoi_v2(char *s, bool *valid);
int		ft_export(char **args);
void	free_cmdtree(t_node *tree);
char	token_peek(char *p);
char	get_token(char **s, char **st, char **et);
t_node	*parse_redirection_create(char *st, char *et, t_node *node, char red);
t_node	*add_redirection(t_redirection *red, t_node *node);
t_node	*diversion(t_node *left, t_node *right, int token);
t_exec	*create_exec();
t_node	*pars_and(char **pcmd);
t_node	*parse_exec(char **pcmd);
bool	fill_file_heredoc(t_redirection *node, char *delim, int fd);
bool	open_herdoc_file(t_redirection *red, t_node *node, int *fd);
t_node	*parse_heredoc(t_node *node, char *start, char *end);
void	substr_print(char *s, char *e, int fd);
void	syntax_error(char *source, char *error, char *start, char *end);

char	**env_without_empty(char **env);
void	sort_2d_array(char ***res);
void	ft_printenv_no_empty(char **env);

void	ft_printenv(char **env);
char	**ptrs_realloc(char **tokens, char *arg);
void	double_free(char **ptrs);
void	print_error(char *source, char *error);
void	print_error_2(char *source, char *arg,char *error);
bool	is_valid_variable_name(char *s);
int		expand_here_doc(int fd, int status, int expand);
void	exec_cmd(t_node *tree, char *prg, char **args);
char	*get_prompt();
int		exit_status(int	status, bool update, bool shift);
void	set_signal_handler(int signal, void (*fun)(int));

void	sigint_handler(int sig);
void sigint_handler2(int sig);
bool	ft_change_last_pro(char ****eenv, char **args);
char	**expander(char *s, int here_doc, int expand, int status);

char	**setup(int argc, char **argv, char **env);
void sigint_handler(int sig);
void sigint_handler2(int sig);
void	set_signal_handler(int signal, void (*fun)(int));
char	**create_env(char **env);
char	**create_new_env();

t_node	*parse_cmd(char *cmd);
void	execute(t_node *node);
bool	in_pipe(int	i);
bool	ft_setenv(char **env, char *name, char *val);
bool	env_is_there(char **env, char *name);
int		ft_close(int fd);
char	*alloc_without_quotes(char *s, int len, int i, int qoute);
char	*delete_quotes(char *s, int i, int len);
bool	itterate_pattern(t_wild_patt *pa, char **p, char **name, int h);
int		check_pattern(char *pattern, char *name, int handle_quot);
char	*patter_true(char *arr, char ***res, int *capacity, char *s);
char	**match_pattern(char *pattern, int handle_quote, int flag, int capacity);
char    *ft_strrealloc2(char *str, size_t size);
char    **ft_realloc(char **lines, char *line);
char	*alloc_for_expand_without_q(char *s, t_elem ***elem);
char	*set_caractere(t_elem *elem, int c);
int    is_exist(int c, char *s);
bool	handle_child_get_pid(char *file);
void	d_free(char **ptr);
char	*get_pid(int pid, int exitt);
char	*alloc_for_elem(t_elem *elem, int here_doc, char *word, char ***res);
char	**concat_two_array(char **res, char **concat);
char	*handle_wild_inside_expand(char *****res, char **sp, int i, char *tmp);
char	*handle_first_in_expand(t_elem **elem, char *s, int *start);
int		is_alpha_num(int c);

#endif
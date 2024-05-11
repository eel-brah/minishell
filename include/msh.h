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

#include "msh_libs.h"
#include "msh_define.h"
#include <termios.h>

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
t_exec	*create_exec(void);
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
void	or_type(t_div *div);
void	and_type(t_div *div);
bool	pipe_type(t_div *div);
bool	red_type(t_redirection *red);
bool	heredoc_type(t_redirection *red);
void	exec_type(t_node *node);
char	**expand_args(char **args);
char	*expand_file(char *file);
static char	*get_cmd(char *prompt);
void	ft_printenv(char **env);
char	**add_ptr_to_ptrs(char **ptrs, char *ptr);
void	double_free(char **ptrs);
void	print_error(char *source, char *error);
void	print_error_2(char *source, char *arg,char *error);
bool	is_valid_variable_name(char *s);
int		expand_here_doc(int fd, int status, int expand);
void	exec_cmd(t_node *tree, char *prg, char **args);
char	*get_prompt(void);
int		exit_status(int	status, bool update, bool shift);
void	set_signal_handler(int signal, void (*fun)(int));
void	sigint_handler(int sig);
void sigint_handler2(int sig);
char	**expander(char *s, int here_doc, int expand, int status);
char	**setup(int argc, char **argv, char **env);
void sigint_handler(int sig);
void sigint_handler2(int sig);
void	set_signal_handler(int signal, void (*fun)(int));
char	**create_env(char **env, size_t size, int add, size_t i);
char	**create_new_env();
void	prg_with_path(char *prg, char **args);
void	execute_prg(char *prg, char **args);
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
char	*handle_env_in_expand(char *env, t_elem **elem, char *s);
char	*handle_wild_in_dollar(char *arr, char *****res);
char	*handle_expand_without_wild(char **sp, char *****res, int i);
char	*handle_dollar(char *s, char ****res, t_elem **elem);
char	**concat_two_array(char **res, char **concat);
char	*split_expand(t_elem **elem, char *s, char ****res);
char	*handl_other_carac(t_elem *elem, char ***res, char *s);
char	*home_set(t_elem **elem);
char	*handle_dollar_special(char *s, t_elem **elem, int status);
char	*shell_name(t_elem **elem);
char	*intial_struct(t_elem *elem, char **word, char ***res, int here_doc);
char	*update_quote(char *s, t_elem *elem);
char	*handle_o_char_sp(t_elem *elem, char *s, char *word, char ***res);
char	*handle_last(t_elem *elem, char ***res, char *word);
char	*edit_env(char *vrbl);
int		is_valid_variable(char *s, int *plus);
bool	add_to_env(char *vrbl);
char	*alloc_without_plus(char *vrbl);
char	*alloc_without_plus(char *vrbl);
bool	add_to_env_plus(char *vrbl);
bool	add_to_env_plus(char *vrbl);
char	*join_variabl(char *old, char *new, char *ptr);
char	*name_file_rand(int number);
t_node	*check_exec(t_exec *cmd, t_node *node, char *pcmd);
t_node	*invalid_red(t_node *node, char *st, char *et);
void	check_path_pwd(char **env, size_t *i, int *add);
bool	handl_path_pwd(char **env, size_t *i, char **p);
bool	reset_term();
#endif
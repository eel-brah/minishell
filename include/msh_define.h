/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_define.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:36:57 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 19:41:05 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_DEFINE_H
# define MSH_DEFINE_H

extern volatile sig_atomic_t got_sigint;
extern char	**environ;

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
	int		here_fd;
	bool	expand;
}	t_redirection;

typedef struct s_elem
{
	int		wild;
	int		status;
	int		index;
	int		here_doc;
	int		capacity;
	int		qoute;
	int		expand;
	int		q;
	int		i;
	char	*arr;
	char	*s;
	char	*tmp;
}	t_elem;

typedef struct wild_patt
{
	char	*wild;
	char	*tmp;
	int		quote;
	int		q;
}	t_wild_patt;


#define WHITESPACES "\t\n\v\f\r "
#define SYMBOL "<>|&()"

#define EXEC	1
#define PIPE	2
#define RED		3
#define AND		4
#define OR		5
#define HEREDOC 6

#define PREMISSIONS 0644
#define GET_STAUS exit_status(0, false, false)

#define WORD 'w'
#define PIPELINE 'p'
#define TAND 'a'
#define TOR 'o'
#define OUT_RED 'r'
#define IN_RED 'R'
#define APP_RED 'd'
#define HERE_DOC 'h'
#define OPEN_PER 't'
#define CLOSE_PER 'T'
#define ERROR 'e'

#endif
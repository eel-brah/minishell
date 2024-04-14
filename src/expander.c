#include "../include/msh.h"

typedef struct s_elem
{
	int		wild;
	int		index;
	int		here_doc;
	int		capacity;
	int		qoute;
	int		q;
	int		i;
	char	*arr;
	char	*tmp;
}	t_elem;

// typedef struct res
// {
// 	char	*tmp;
// }	t_res;


char	**match_pattern(char *pattern, int handle_quote);
char	*delete_quotes(char *s);
char	*alloc_for_expand_without_q(char *s, t_elem ***elem);
char	**concat_two_array(char **res, char **concat);
char    *ft_strrealloc2(char *str, size_t size);

char	*set_caractere(t_elem *elem, int c)
{
	char	*tmp;
	if (elem->index == elem->capacity - 1)
	{
		// printf("elem capacity %d\n", elem->capacity);
		elem->capacity *= 2;
		tmp = ft_strrealloc2(elem->arr, elem->capacity);
		if (!tmp)
			return (perror("malloc"), NULL);
		elem->arr = tmp;
	}
	elem->arr[elem->index] = c;
	elem->index++;
	// elem->i++;
	return ((char *)42);
}

int	is_alpha_num(int c)
{
	if (c <= 'z' && c >= 'a')
		return (1);
	if (c <= 'Z' && c >= 'A')
		return (1);
	if (c <= '9' && c >= '0')
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

static char    **handle_case(char **lines, char *line)
{
    lines = malloc(sizeof(char *) * 2);
    if (!lines)
		return (NULL);
    lines[0] = line;
    lines[1] = NULL;
    return (lines);
}

int    is_exist(int c, char *s)
{
    int    i;

    i = 0;
    while (s[i])
	{
        if (s[i++] == c)
            return (1);
	}
	return (0);
}


void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char    **ft_realloc(char **lines, char *line)
{
    int        i;
    char    **tmp;

	if (!lines)
        return (handle_case(lines, line));
    i = 0;
    tmp = lines;
    while (lines[i])
        i++;
    lines = malloc(sizeof(char *) * (i + 2));
	if (!lines)
			return (NULL);
    i = 0;
    while (tmp[i])
    {
        lines[i] = tmp[i];
        i++;
    }
    lines[i] = line;
    lines[i + 1] = NULL;
    free(tmp);
    return (lines);
}

// char	*ft_concat(char *word, char c)
// {
// 	int		i;
// 	char	*res;
// 	int		len;

// 	i = 0;
// 	len = 0;
// 	if (word == NULL)
// 	{
// 		res = malloc(2);
// 		if (!res)
// 			return (NULL);
// 		res[0] = c;
// 		res[1] = '\0';
// 	}
// 	else
// 	{
// 		while (word[len])
// 			len++;
// 		res = malloc(len + 2);
// 		if (!res)
// 			return (NULL);
// 		while (i < len)
// 		{
// 			res[i] = word[i];
// 			i++;
// 		}
// 		res[i++] = c;
// 		res[i] = '\0';
// 	}
// 	free(word);
// 	return (res);
// }

// char	*ft_concat(char *word, char *s, int start, int end, int inquote, char quoute)
// {
// 	word = malloc((end - start) + 1);
// 	while (start < end)
// 	{
// 		if (inquote || s[start] )
// 		word[start] = s[start];
// 		start++;
// 	}
// }

char    *ft_strrealloc2(char *str, size_t size)
{
    char    *new_str;

    new_str = ft_calloc(size, sizeof(char));
    if (!new_str)
        return (NULL);
    ft_strlcat(new_str, str, size);
    free(str);
    return (new_str);
}
char	*handle_dollar_special(char *s, t_elem **elem, int status)
{
	char	*num;
	int		j;
	j = 0;
	
	//WEXITSTATUS  (((*(int *)&(status)) >> 8) & 0x000000ff)
	((*elem)->i)++;
	if (s[(*elem)->i] != '?')
	{
		if (set_caractere(*elem, s[(*elem)->i]) == NULL)
			return (NULL);
		return (s);
	}
	num = ft_itoa(WEXITSTATUS(status));
	if (!num)
		return (NULL);
	while(num[j])
	{
		if (set_caractere(*elem, num[j]) == NULL)
			return (NULL);
		j++;
	}
	free(num);
	return (s);
}

char	*handle_wild_inside_expand(char *****res, char **sp, int i)
{
	char	*tmp;
	char	**tmp1;
	char	**exp;

	exp = match_pattern(sp[i], 0);
	if (!exp)
		return (free_arr(sp), perror("malloc "), NULL);
	else if (exp == (char **)42)
	{
		tmp = ft_strdup(sp[i]);
		if (!tmp)
			return (free(sp), perror("malloc "), NULL);
		tmp1 = ft_realloc(***res, tmp);
		if (!tmp1)
			return (free_arr(***res), ***res = NULL, free_arr(sp), perror("malloc "), NULL);
		else
			***res = tmp1;
	}
	else
	{
		tmp1 = concat_two_array(***res, exp);
		if (tmp1 == NULL)
			return (perror("malloc "), free_arr(sp), NULL);
		***res = tmp1;
	}
	return ((char *)42);
}

char	*handle_expand_without_wild(char **sp, char *****res, int i)
{
	char	*tmp;
	char	**tmp1;

	tmp = ft_strdup(sp[i]);
	if (!tmp)
		return (free(sp), perror("malloc "), NULL);
	tmp1 = ft_realloc(***res, tmp);
	if (!tmp1)
		return (free_arr(***res), ***res = NULL, free_arr(sp), perror("malloc "), NULL);
	else
		***res = tmp1;
	return ((char *)42);
}
char	*handle_wild_in_dollar(char *arr,char *****res)
{
	char	**sp;
	int		i;
	// char	*tmp;
	// char	**tmp1;
	// char	**exp;

	i = 0;
	sp = ft_split(arr, ' ');
	if (!sp)
		return (NULL);
	while (sp[i])
	{
		if (ft_strchr(sp[i], '*'))
		{
			if (handle_wild_inside_expand(res, sp, i) == NULL)
				return (NULL);
		}
		else
		{
			if (handle_expand_without_wild(sp, res, i) == NULL)
				return (NULL);
		}
		i++;
	}
	return (free_arr(sp), (char *)42);
}
char	*handle_first_in_expand(t_elem **elem, char *s, int *start)
{
	(*elem)->i++;
	if (s[(*elem)->i] >= '0' && s[(*elem)->i] <= '9')
		return (s);
	*start = 0;
	if (s[(*elem)->i] == '\0')
	{
		(*elem)->i--;
		(*elem)->arr[(*elem)->index] = s[(*elem)->i];
		(*elem)->index++;
		return (s);
	}
	while (s[(*elem)->i + *start] && is_alpha_num(s[(*elem)->i + *start]))
		(*start)++;
	if (*start == 0 && ((*elem)->qoute == 1 || (*elem)->here_doc))
	{
		(*elem)->i--;
		(*elem)->arr[(*elem)->index] = s[(*elem)->i];
		((*elem)->index)++;
		return (s);
	}
	return (NULL);
}

char	*handle_env_in_expand(char *env, t_elem **elem, char *s)
{
	int		j;

	if (!env)
	{
		if (!is_alpha_num(s[(*elem)->i + 1]) && s[(*elem)->i + 1] != '\0' && s[(*elem)->i] == '$' && s[(*elem)->i + 1] != '\'' && s[(*elem)->i + 1] != '"')
			(*elem)->arr[((*elem)->index)++] = s[(*elem)->i];
		return (s);
	}
	j = 0;
	// if (ft_strchr(env, '\'') == NULL)
	// 	a = '\'';
	// else if(ft_strchr(env, '"') == NULL)
	// 	a = '"';
	if ((*elem)->here_doc == 0 && set_caractere(*elem, 20) == NULL)
			return (NULL);
	while (env[j])
	{
		if (set_caractere(*elem, env[j]) == NULL)
			return (NULL);
		j++;
	}
	if ((*elem)->here_doc == 0 && set_caractere(*elem, 20) == NULL)
			return (NULL);
	return (NULL);
}
char	*split_expand(t_elem **elem, char *s, char ****res)
{
	char	*exp;

	if ((*elem)->qoute == 0 && (*elem)->here_doc == 0)
	{
		(*elem)->tmp = alloc_for_expand_without_q(s, &elem);
		if ((*elem)->tmp == (char *)16)
			return (s);
		else if ((*elem)->tmp == NULL)
			return (NULL);
		exp = delete_quotes((*elem)->arr);
		if (!exp)
			return (NULL);
		(*elem)->tmp = handle_wild_in_dollar(exp, &res);
		if ((*elem)->tmp == NULL)
			return (NULL);
		ft_memset((*elem)->arr, 0, (*elem)->index);
		(*elem)->index = 0;
	}
	return (s);
}
char	*handle_dollar(char *s, char ****res, t_elem **elem)
{
	int		j;
	char	*exp;
	char	*env;
	int		start;

	if (handle_first_in_expand(elem, s, &start) != NULL)
		return (s);
	exp = malloc(start + 1);
	if (!exp)
		return (NULL);
	j = 0;
	while (j < start)
	{
		exp[j] = s[(*elem)->i + j];
		j++;
	}
	exp[j] = '\0';
	((*elem)->i) = ((*elem)->i) + start - 1;
	env = getenv(exp);
	free(exp);
	if (handle_env_in_expand(env, elem, s) != NULL)
		return (s);
	if (split_expand(elem, s, res) == NULL)
		return (NULL);
	return (s);
}


char	*alloc_for_expand_without_q(char *s, t_elem ***elem)
{
	int		qoute;
	int		q;
	int		tmp;
	int		tmp1;
	int		tmp2;



	qoute = (**elem)->qoute;
	q = (**elem)->q;
	tmp = (**elem)->i;
	tmp1 = (**elem)->index;
	tmp2 = tmp1;
	((**elem)->i)++;
	while (s[((**elem)->i)] != '\0' && !is_exist(s[((**elem)->i)], "\t\n\v\f\r  "))
	{
		if (s[((**elem)->i)] == '$')
		{
			(**elem)->qoute = qoute ;
			(**elem)->q = q;
			(**elem)->index = tmp1;
			while (tmp1 < tmp2)
				(**elem)->arr[tmp1++] = 0;
			(**elem)->i = tmp;
			// ((**elem)->i)--;
			return ((char *)16);
		}
		if ((s[((**elem)->i)] == '\'' || s[((**elem)->i)] == '\"' ) && ((**elem)->qoute == 0 || (**elem)->q == s[((**elem)->i)]))
		{
			if ((**elem)->qoute == 0)
			{
				// tmp = (**elem)->i;
				// tmp1 = (**elem)->index;
				// tmp2 = tmp1;
				(**elem)->q = s[((**elem)->i)];
				(**elem)->qoute = 1;
			}
			else
			{
				(**elem)->qoute = 0;
				(**elem)->q = '\0';
			}
			((**elem)->i)++;
			continue ;
		}
		else
		{
			if (set_caractere(**elem, s[((**elem)->i)]) == NULL)
				return (NULL);
			(((**elem)->i))++;
		}
	}
	((**elem)->i)--;
	return ((char *)42);
}
char	*alloc_without_quotes(char *s, int len)
{
	int		qoute;
	int		q;
	char	*res;
	int		i;
	int		f;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	len = 0;
	f = 1;
	qoute = 0;
	q = '\0';
	while (s && s[i])
	{
		if (s[i] == 20 && f == 1) 
		{
			i++;
			f = 0;
			continue;
		}
		if (s[i] == 20 && f == 0)
		{
			i++;
			f = 1;
			continue;
		}
		if ((s[i] == '\'' || s[i] == '\"' ) && (qoute == 0 || q == s[i]) && f)
		{
			if (qoute == 0)
			{
				q = s[i];
				qoute = 1;
			}
			else
			{
				qoute = 0;
				q = '\0';
			}
			i++;
			continue ;
		}
		else
		{
			res[len++] = s[i];
			i++;
		}
	}
	res[len] = '\0';
	return (res);
}
char	**concat_two_array(char **res, char **concat)
{
	char	**result;
	int		len;
	int		i;

	len = 0;
	while (res && res[len])
		len++;
	i = 0;
	while (concat && concat[i])
		i++;
	result = malloc(sizeof(char *) * (len + i + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (res && res[i])
	{
		result[i] = res[i];
		i++;
	}
	len = 0;
	while (concat && concat[len])
	{
		result[i] = concat[len];
		len++;
		i++;
	}
	free(res);
	free(concat);
	result[i] = NULL;
	return (result);
}
char	*alloc_for_elem(t_elem *elem, int here_doc, char *word, char ***res)
{
	char	**tmp;
	char	**concat;
	int 	flag;


	flag = 1;
	tmp = NULL;
	concat = NULL;
	if (elem->wild && here_doc == 0)
	{
		concat = match_pattern(elem->arr, 1);
		if (concat == (char **)42)
			word = delete_quotes(elem->arr);
		else if (concat == NULL)
			return (perror("malloc sdf"), free(elem->arr), free_arr(*res), NULL);
		else
		{
			tmp = concat_two_array(*res, concat);
			if (tmp == NULL)
				return (perror("malloc fff"), free(elem->arr), free_arr(*res), NULL);
			*res = tmp;
			flag = 0;
		}
		elem->wild = 0;

	}
	else if (here_doc == 0)
		word = delete_quotes(elem->arr);
	else
		word = ft_strdup(elem->arr);
	if (flag)
	{
		if (!word)
			return (perror("malloc dsdf"), free(elem->arr), free_arr(*res), NULL);
		tmp = ft_realloc(*res, word);
		if (!tmp)
			return (free_arr(*res) , free(word), free(elem->arr), perror("malloc "), NULL);
		else
			*res = tmp;
	}
	elem->index = 0;
	ft_memset(elem->arr, 0, elem->capacity);
	return ((char *)42);
}

char	*delete_quotes(char *s)
{
	int		qoute;
	int		q;
	int		f;
	int		i;
	int		len;

	i = 0;
	len = 0;
	qoute = 0;
	q = '\0';
	f = 1;
	while (s && s[i])
	{
		if (s[i] == 20 && f == 1) 
		{
			i++;
			f = 0;
			continue;
		}
		if (s[i] == 20 && f == 0)
		{
			i++;
			f = 1;
			continue;
		}
		if ((s[i] == '\'' || s[i] == '\"' ) && (qoute == 0 || q == s[i]) && f)
		{
			if (qoute == 0)
			{
				q = s[i];
				qoute = 1;
			}
			else
			{
				qoute = 0;
				q = '\0';
			}
			i++;
			continue ;
		}
		else
		{
			i++;
			len++;
		}
	}
	return (alloc_without_quotes(s, len));
}
char	*handl_other_carac(t_elem *elem, char ***res, int here_doc, int expand, char *s, int status)
{
	// char	*arrt;

	// if (elem->index == elem->capacity - 1)
	// {
	// 	elem->capacity *= 2;
	// 	arrt = ft_strrealloc2(elem->arr, elem->capacity);
	// 	if (!arrt)
	// 		return (free(elem->arr), free_arr(*res), perror("malloc"), NULL);
	// 	elem->arr = arrt;
	// }
	if (s[elem->i] == '*' && elem->qoute == 0)
	{
		elem->wild = 1;
	}
	if (s[elem->i] == '$' && (elem->q == '\"' || elem->q == '\0' || here_doc) && is_exist(s[elem->i + 1], "*@#?	$-!0") && expand)
	{
		if (handle_dollar_special(s, &elem, status) == NULL)
			return (free(elem->arr), free_arr(*res), *res = NULL, perror("malloc "), NULL);
	}
	else if (s[elem->i] == '$' && (elem->q == '\"' || elem->q == '\0' || here_doc) && expand)
	{
		if (handle_dollar(s, &res, &elem) == NULL)
			return (free(elem->arr), free_arr(*res), *res = NULL, perror("malloc "), NULL);
	}
	else
	{
		if (set_caractere(elem, s[elem->i]) == NULL)
			return (free(elem->arr), free_arr(*res), *res = NULL, perror("malloc "), NULL);
		// elem->arr[elem->index++] = s[elem->i];
	}
	return ((char *)42);
}
char	*intial_struct(t_elem *elem, char **word, char ***res, int here_doc)
{
	elem->q = '\0';
	elem->here_doc = here_doc;
	elem->qoute = 0;
	*word = NULL;
	*res = NULL;
	elem->index = 0;
	elem->i = 0;
	elem->wild = 0;
	elem->capacity = 1024;
	elem->arr = malloc(elem->capacity);
	if (!elem->arr)
		return (perror("malloc"), NULL);
	ft_memset(elem->arr, 0, elem->capacity);
	return ((char *)42);
}

char	*update_quote(char *s, t_elem *elem)
{
	if (elem->qoute == 0)
	{
		elem->q = s[elem->i];
		elem->qoute = 1;
	}
	else
	{
		elem->qoute = 0;
		elem->q = '\0';
	}
	if (set_caractere(elem, s[elem->i]) == NULL)
		return (NULL);
	return ((char *) 42);
}

char	*handle_last(t_elem *elem, char ***res, char *word)
{
	if (elem->index > 0)
	{
		if (alloc_for_elem(elem, elem->here_doc, word, res) == NULL)
			return (NULL);
	}
	else
	{
		if (!*res)
		{
			*res = malloc(sizeof(char *) * 1);
			if (!*res)
				return (free(elem->arr), NULL);
			(*res)[0] = NULL;
		}
	}
	return ((char *)42);
}
char	*handle_other_carac_space(t_elem *elem, char *s, char *word, int expand, char ***res, int status)
{
	if (is_exist(s[elem->i], "\t\n\v\f\r ") && elem->qoute == 0 && elem->index != 0 && !elem->here_doc)
	{
		if (alloc_for_elem(elem, elem->here_doc, word, res) == NULL)
			return (NULL);
	}
	else if(is_exist(s[elem->i], "\t\n\v\f\r ") && elem->here_doc == 1)
	{
		if (set_caractere(elem, s[elem->i]) == NULL)
			return (NULL);
	}
	else if (!is_exist(s[elem->i], "\t\n\v\f\r ") || elem->qoute == 1)
	{
		if (handl_other_carac(elem, res, elem->here_doc, expand, s, status) == NULL)
			return (NULL);
	}
	return (s);
}
char	**expander(char *s, int here_doc, int expand, int status)
{
	char	**res;
	char	*word;
	t_elem	elem;
	
	if (intial_struct(&elem, &word, &res, here_doc) == NULL)
		return (NULL);
	while (s && s[elem.i])
	{
		if ((s[elem.i] == '\'' || s[elem.i] == '\"' ) && (elem.qoute == 0 || elem.q == s[elem.i]))
		{
			if (update_quote(s, &elem) == NULL)
				return (free(elem.arr), free_arr(res), NULL);
		}
		else
		{
			// if (is_exist(s[elem.i], "\t\n\v\f\r ") && elem.qoute == 0 && elem.index != 0)
			// {
			// 	if (alloc_for_elem(&elem, here_doc, word, &res) == NULL)
			// 		return (NULL);
			// }
			// else if (!is_exist(s[elem.i], "\t\n\v\f\r ") || elem.qoute == 1)
			// {
			// 	if (handl_other_carac(&elem, &res, here_doc, expand, s) == NULL)
			// 		return (NULL);
			// }
			if (handle_other_carac_space(&elem, s, word, expand, &res, status) == NULL)
				return (NULL);
		}
		elem.i++;
	}
	if (handle_last(&elem, &res, word) == NULL)
		return (NULL);
	return (free(elem.arr), res);
}

void	xx()
{
	system("leaks a.out");
}

/*  *********  */
#include <dirent.h>

int	check_pattern(char *pattern, char *name, int handle_quot)
{
	char	*wild;
	char	*tmp;
	int		quote;
	int		q;

	quote = 0;
	q = '\0';
	wild = NULL;
	while (*name)
	{
		if ((*pattern == '"' || *pattern == '\'') && (quote == 0 || q == *pattern) && handle_quot) {
			if (quote == 0)
			{
				quote = 1;
				q = *pattern;
			}
			else
			{
				quote = 0;
				q = '\0';
			}
        	pattern++;
            continue;
        }
		if (*pattern == '*' && quote == 0)
		{
			wild = pattern++;
			tmp = name + 1;
		}
		else if (*pattern == *name)
		{
			pattern++;
			name++;
		}
		else
		{
			if (wild)
			{
				pattern = wild + 1;
				if (*pattern == q && quote == 1)
				{
					quote = 0;
					q = '\0';
				}
				
				name = tmp++;
			}
			else
				return (0);
		}
	}
	if (quote && *pattern == q)
	{
		pattern++;
		quote = 0;
	}
	while (*pattern == '\'' || *pattern == '"')
		pattern++;
	while (*pattern == '*' && quote == 0)
		pattern++;
	return (*pattern == '\0');
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}


void	sort_2d_array(char ***res)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while ((*res)[i] != NULL)
	{
		j = i + 1;
		while ((*res)[j] != NULL)
		{
			if (ft_strcmp((*res)[i], (*res)[j]) > 0)
			{
				tmp = (*res)[i];
				(*res)[i] = (*res)[j];
				(*res)[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**match_pattern(char *pattern, int handle_quote)
{
	DIR				*dir;
	struct dirent	*st;
	int				flag;
	char			*arr;
	char			**res;
	char			**rest;
	char			*arrt;
	int				i;
	int				index;
	int				capacity;

	flag = 1;
	index = 0;
	capacity = 1024;
	dir = opendir(".");
	arr = malloc(capacity);
	res = NULL;
	if (!arr)
		return (NULL);
	ft_memset(arr, 0, capacity);
	while ((st = readdir(dir)) != NULL)
	{
		if (st->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (check_pattern(pattern, st->d_name, handle_quote) == 1)
		{
			flag = 0;
			i = 0;
			index = 0;
			while (st->d_name[i])
			{
				if (index == capacity - 1)
				{
					capacity *= 2;
					arrt = ft_strrealloc2(arr, capacity);
					if (!arrt)
						return (free(arr), closedir(dir), NULL);// cheack if return nULL and free old arr
					arr = arrt;
				}
				arr[index] = st->d_name[i];
				index++;
				i++;
			}
			arrt = ft_strdup(arr);
			if (!arrt)
				return (free(arr), NULL);
			rest = ft_realloc(res, arrt);
			if (!rest)
				return (free(arr), NULL);
			res = rest;
			ft_memset(arr, 0, capacity);
		}
	}
	closedir(dir);
	if (flag)
		return (free(arr), (char **)42);
	sort_2d_array(&res);
	return (free(arr), res);
}

int	expand_here_doc(int fd, int status, int expand)
{
	char	*s;
	char	**res;
	int		fd_res;
	int		fd_ret;

	fd_res = open("fd_res", O_CREAT | O_RDWR | O_TRUNC, PREMISSIONS);
	fd_ret = open("fd_res", O_CREAT | O_RDWR | O_TRUNC, PREMISSIONS);
	if (fd_res == -1 || fd_ret == -1)
		return (perror("open"), -1); // close 
	unlink("fd_res");
	while (1)
	{
		// check if get_next fails
		s = get_next_line(fd);
		if (!s)
			break ;
		res = expander(s, 1, expand, status);
		if (!res)
			return (close(fd_res), free(s), -2);
		write(fd_res, *res, ft_strlen(*res));
		double_free(res);
		free(s);
	}
	close(fd_res);
	return (fd_ret);
}

bool	ft_change_last_pro(char ****eenv, char **args)
{
	int		len;
	char	*s;
	char	**res;
	bool	ret = true;


	// printf("hereheresdfdsf\n");
	(void)eenv;
	len = 0;
	while (args && args[len])
		len++;
	if (len > 0)
		len--;
	s = ft_strjoin("_=", args[len]);
	res = malloc(sizeof(char *) * 2);
	if (!res)
		return (free(s), false);
	// printf("export %s\n",s);
	res[0] = s;
	res[1] = NULL;
	if (!s)
		return (false);
	// ret = ft_export(&environ, res);
	free(s);
	return (ret);
}
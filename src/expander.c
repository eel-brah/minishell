#include "../include/msh.h"

// typedef struct res
// {
// 	char	*tmp;
// }	t_res;


char	**match_pattern(char *pattern, int handle_quote, int flag, int capacity);
char	*delete_quotes(char *s, int i, int len);
char	*delete_quotes_in(char *s);
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
	char *pid = get_pid(0, 0);
	((*elem)->i)++;
	if (s[(*elem)->i] == '$')
	{
		int i = 0;
		while (pid && pid[i] && pid[i] != '\n')
		{
			if (set_caractere(*elem, pid[i]) == NULL)
				return (NULL);
			i++;
		}
		return (s);
	}
	if (s[(*elem)->i] != '?')
	{
		if (set_caractere(*elem, s[(*elem)->i]) == NULL)
			return (NULL);
		return (s);
	}
	// num = ft_itoa(WEXITSTATUS(status));
	num = ft_itoa(status >> 8);
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
	char	*tmp;
	// char	**tmp1;
	// char	**exp;

	i = 0;
	tmp = NULL;
	sp = ft_split(arr, ' ');
	if (!sp)
		return (NULL);
	while (sp[i])
	{
		if (ft_strchr(sp[i], '*'))
		{
			if (handle_wild_inside_expand(res, sp, i, tmp) == NULL)
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
		exp = delete_quotes((*elem)->arr, 0, 0);
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
	// if ((*elem)->expand == 0)
	// 	return (s);
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
		(1) && (result[i] = res[i], i++);
	len = 0;
	while (concat && concat[len])
	{
		result[i] = concat[len];
		len++;
		i++;
	}
	return (free(res), free(concat), result[i] = NULL, result);
}

char	*handl_other_carac(t_elem *elem, char ***res, int here_doc, int expand, char *s, int status)
{
	if (s[elem->i] == '*' && elem->qoute == 0)
	{
		elem->wild = 1;
	}
	if (s[elem->i] == '$' && (elem->q == '\"' || elem->q == '\0' || here_doc) && is_exist(s[elem->i + 1], "*@#?	$-!0") && expand)
	{
		if (handle_dollar_special(s, &elem, status) == NULL)
			return (free(elem->arr), free_arr(*res), *res = NULL, perror("malloc "), NULL);
	}
	else if (s[elem->i] == '$' && (elem->q == '\"' || elem->q == '\0' || here_doc))
	{
		if (handle_dollar(s, &res, &elem) == NULL)
			return (free(elem->arr), free_arr(*res), *res = NULL, perror("malloc "), NULL);
	}
	else
	{
		if (set_caractere(elem, s[elem->i]) == NULL)
			return (free(elem->arr), free_arr(*res), *res = NULL, perror("malloc "), NULL);
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
	elem.expand = expand;
	while (s && s[elem.i])
	{
		if ((s[elem.i] == '\'' || s[elem.i] == '\"' ) && (elem.qoute == 0 || elem.q == s[elem.i]))
		{
			if (update_quote(s, &elem) == NULL)
				return (free(elem.arr), free_arr(res), NULL);
		}
		else
		{
			if (handle_other_carac_space(&elem, s, word, expand, &res, status) == NULL)
				return (NULL);
		}
		elem.i++;
	}
	if (handle_last(&elem, &res, word) == NULL)
		return (NULL);
	return (free(elem.arr), res);
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
	while (res && (*res)[i] != NULL)
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


int	expand_here_doc(int fd, int status, int expand)
{
	char	*s;
	int		count;
	char	*name;
	char	**res;
	int		fd_res;
	int		fd_ret;

	count = 69;
	name = ft_itoa((int)&expand_here_doc);
	if (!name)
		return (perror("malloc"), -1);
	fd_res = open(name, O_CREAT | O_RDWR | O_TRUNC | O_EXCL, PREMISSIONS);
	while (fd_res == -1  && errno == EEXIST)
	{
		free(name);
		name = ft_itoa((int)&expand_here_doc + count);
		if (!name)
			return (perror("malloc"), -1);
		fd_res = open(name, O_CREAT | O_RDWR | O_TRUNC | O_EXCL, PREMISSIONS);
		count++;
	}
	fd_ret = open(name, O_RDWR, PREMISSIONS);
	unlink(name);
	if (fd_res == -1 || fd_ret == -1)
		return (perror("open"), free(name), close(fd_res), close(fd_ret), -1);
	while (1)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		res = expander(s, 1, expand, status);
		if (!res)
			return (close(fd_res), close(fd_ret), free(name), free(s), -1);
		write(fd_res, *res, ft_strlen(*res));
		double_free(res);
		free(s);
	}
	return (close(fd_res), free(name), fd_ret);
}

bool	ft_change_last_pro(char ****eenv, char **args)
{
	int		len;
	char	*s;
	char	**res;
	bool	ret = true;


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
	res[0] = s;
	res[1] = NULL;
	if (!s)
		return (false);
	free(s);
	return (ret);
}

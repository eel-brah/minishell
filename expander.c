
#include "include/msh.h"

typedef struct elem
{
	int	wild;
	int	index;
	int	here_doc;
	int	capacity;
	int	qoute;
	int	q;
}	t_elem;

char	*match_pattern(char *pattern);
char	*delete_quotes(char *s, int index);

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
        if (s[i++] == c)
            return (1);
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

char    *ft_strrealloc(char *str, size_t size)
{
    char    *new_str;

    new_str = ft_calloc(size, sizeof(char));
    if (!new_str)
        return (NULL);
    ft_strlcat(new_str, str, size);
    free(str);
    return (new_str);
}
void	handle_dollar_special(char *s, int *i, char *arr, int *index, int capacity)
{
	// if (s[*i] == '\'')
	// 	(*i)++;
	//printf("Special caractere \n");
	(void)s;
	(void)i;
	(void)arr;
	(void)index;
	(void)capacity;
}
char	*handle_dollar(char *s, int *i, char *arr, int *index, int capacity, int inquote, int here_doc)
{
	int		j;
	char	*exp;
	char	*env;
	int		start;
	// int		d;

	(*i)++;
	if (s[*i] >= '0' && s[*i] <= '9')
		return (s);
	start = 0;
	if (s[*i] == '\0')
	{
		(*i)--;
		arr[*index] = s[*i];
		(*index)++;
		return (s);
	}
	while (s[*i + start] && is_alpha_num(s[*i + start]))
		start++;
	if (start == 0 && (inquote == 1 || here_doc))
	{
		(*i)--;
		arr[*index] = s[*i];
		(*index)++;
		return (s);
	}
	exp = malloc(start + 1);
	if (!exp)
		return (NULL);
	j = 0;
	while (j < start)
	{
		exp[j] = s[*i + j];
		j++;
	}
	exp[j] = '\0';
	(*i) = (*i) + start - 1;
	env = getenv(exp);
	free(exp);
	if (!env)
		return (s);
	j = 0;
	while (env[j])
	{
		if (*index == capacity - 1)
		{
			capacity *= 2;
			arr = ft_strrealloc(arr, capacity);
			if (!arr)
				return (NULL);
		}
		arr[*index] = env[j];
		j++;
		(*index)++;
	}
	return (s);
}

char	*alloc_without_quotes(char *s, int len)
{
	int		qoute;
	int		q;
	char	*res;
	int		i;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	len = 0;
	qoute = 0;
	q = '\0';
	while (s && s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"' ) && (qoute == 0 || q == s[i]))
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

char	*alloc_for_elem(t_elem *elem, int here_doc, char **arr, char *word, char ***res)
{
	char	**tmp;

	if (elem->wild && here_doc == 0)
	{
		word = match_pattern(*arr);
		if (word == (char *)42)
			word = ft_strdup(*arr);
		elem->wild = 0;
	}
	else if (here_doc == 0)
		word = delete_quotes(*arr, elem->index);
	else
		word = ft_strdup(*arr);
	if (!word)
		return (perror("malloc"), free(*arr), free_arr(*res), NULL);
	elem->index = 0;
	tmp = ft_realloc(*res, word);
	if (!tmp)
		return (free_arr(*res) , free(word), free(*arr), perror("malloc "), NULL);
	else
		*res = tmp;
	ft_memset(*arr, 0, elem->capacity);
	return ((char *)42);
}

char	*delete_quotes(char *s, int index)
{
	int		qoute;
	int		q;
	int		i;
	int		len;

	i = 0;
	len = 0;
	qoute = 0;
	q = '\0';
	(void)index;
	while (s && s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"' ) && (qoute == 0 || q == s[i]))
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
char	*handl_other_carac(t_elem *elem, char *arr, char **res, int here_doc, int expand, char *s, int *i)
{
	char	*arrt;

	if (elem->index == elem->capacity - 1)
	{
		elem->capacity *= 2;
		arrt = ft_strrealloc(arr, elem->capacity);
		if (!arrt)
			return (free(arr), free_arr(res), perror("malloc"), NULL);
		arr = arrt;
	}
	if (s[*i] == '*' && elem->qoute == 0)
	{
		elem->wild = 1;
	}
	if (s[*i] == '$' && (elem->q == '\"' || elem->q == '\0' || here_doc) && is_exist(s[*i + 1], "*@#?	$-!0") && expand)
		handle_dollar_special(s, i, arr, &elem->index, elem->capacity);
	else if (s[*i] == '$' && (elem->q == '\"' || elem->q == '\0' || here_doc) && expand)
	{
		if (handle_dollar(s, i, arr, &elem->index, elem->capacity, elem->qoute, here_doc) == NULL)
			return (free(arr), free_arr(res), perror("malloc"), NULL);
	}
	else
		arr[elem->index++] = s[*i];
	return ((char *)42);
}
char	**expander(char *s, int here_doc, int expand)
{
	char	**res;
	// char	**tmp;
	char	*word;
	char	*arr;
	// char	*arrt;
	int		i;
	t_elem	elem;
	
	elem.q = '\0';
	elem.qoute = 0;
	i = 0;
	word = NULL;
	res = NULL;
	elem.index = 0;
	elem.wild = 0;
	elem.capacity = 1024;
	arr = malloc(elem.capacity);
	if (!arr)
		return (perror("malloc"), NULL);
	ft_memset(arr, 0, elem.capacity);
	while (s && s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"' ) && (elem.qoute == 0 || elem.q == s[i]))
		{
			if (elem.qoute == 0)
			{
				elem.q = s[i];
				elem.qoute = 1;
			}
			else
			{
				elem.qoute = 0;
				elem.q = '\0';
			}
			arr[elem.index++] = s[i];
			i++;
			continue ;
		}
		else
		{
			if (is_exist(s[i], "\t\n\v\f\r ") && elem.qoute == 0 && elem.index != 0)
			{
				if (alloc_for_elem(&elem, here_doc, &arr, word, &res) == NULL)
					return (NULL);
			}
			else if (!is_exist(s[i], "\t\n\v\f\r ") || elem.qoute == 1)
			{
				if (handl_other_carac(&elem, arr, res, here_doc, expand, s, &i) == NULL)
					return (NULL);
			}
		}
		i++;
	}
	if (elem.index > 0)
	{
		if (alloc_for_elem(&elem, here_doc, &arr, word, &res) == NULL)
			return (NULL);
	}
	if (elem.qoute == 1 && here_doc == 0)
	{
		printf("syntax error\n");
		return (free_arr(res), free(arr), NULL);
	}
	return (free(arr), res);
}

void	xx()
{
	system("leaks a.out");
}

/*  *********  */
#include <dirent.h>

int	check_pattern(char *pattern, char *name)
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
		if ((*pattern == '"' || *pattern == '\'') && (quote == 0 || q == *pattern)) {
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
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (-1);
		i++;
	}
	return (s1[i] - s2[i]);
}
char	*match_pattern(char *pattern)
{
	DIR				*dir;
	struct dirent	*st;
	int				flag;
	char			*arr;
	char			*arrt;
	int				i;
	int				index;
	int				capacity;

	flag = 1;
	index = 0;
	// printf("here\n");
	capacity = 1024;
	dir = opendir(".");
	arr = malloc(capacity);
	if (!arr)
		return (NULL);
	ft_memset(arr, 0, capacity);
	while ((st = readdir(dir)) != NULL)
	{
		if (ft_strcmp(st->d_name, ".") == 0)
			continue ;
		if (ft_strcmp(st->d_name, "..") == 0)
			continue ;
		if (st->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (check_pattern(pattern, st->d_name) == 1)
		{
			flag = 0;
			i = 0;
			while (st->d_name[i])
			{
				if (index == capacity - 1)
				{
					capacity *= 2;
					arrt = ft_strrealloc(arr, capacity);
					if (!arrt)
						return (free(arr), closedir(dir), NULL);// cheack if return nULL and free old arr
					arr = arrt;
				}
				arr[index] = st->d_name[i];
				index++;
				i++;
			}
			arr[index++] = ' ';
		}
	}
	closedir(dir);
	if (!flag)
		arr[index--] = '\0';
	else
		return ((char *)42);
	return (arr);
}
int main()
{

	char	**res;
	char *s;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	// cheak("*");
	// match_pattern("*", "name");
 	// s = "\'hello      world    \'    hhhh";
	// res = handle_quotes(s);
	// while (res[i])
	// {
	// 	printf("%s\n",res[i]);
	// 	i++;
	// }
	// atexit(xx);
	while (j < 15)
	{
		s = readline("?? : ");
		if (s && *s)
		{
			add_history(s);
			res = expander(s, 0, 1);
			// if (!res)
			// 	printf("nULL RE\n");
			while (res && res[i])
			{
				printf("%s\n",res[i]);
				i++;
			}
			i = 0;
			free(s);
			free_arr(res);
		}
		// match_pattern(s);
		j++;
	}
	return 0; 
}
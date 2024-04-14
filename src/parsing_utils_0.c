#include "../include/msh.h"

bool	get_token_cmp2(char *p, char *r)
{
	if (!ft_strncmp(p, "&&", 2))
		*r = OAND;
	else if (!ft_strncmp(p, "||", 2))
		*r = OOR;
	else if (!ft_strncmp(p, "<<", 2))
		*r = HERE_DOC;
	else if (!ft_strncmp(p, ">>", 2))
		*r = APP_RED;
	else
		return (false);
	return (true);
}

bool	get_token_cmp1(char *p, char *r)
{
	if (*p == '|')
		*r = PIPELINE;
	else if (*p == '>')
		*r = OUT_RED;
	else if (*p == '<')
		*r = IN_RED;
	else if (*p == '(')
		*r = OPEN_PER;
	else if (*p == ')')
		*r = CLOSE_PER;
	else
		return (false);
	return (true);
}

void	get_token_word(char **dp, char *r)
{
	char	*p;
	bool	Q;
	char	q;

	Q = false;
	p = *dp;
	while (*p && ((!ft_strchr(WHITESPACES, *p) && (!ft_strchr(SYMBOL, *p)
		|| (*p == '&' && *(p + 1) != '&'))) || Q))
	{
		if ((*p == '\"' || *p == '\'') && (!Q || (Q && *p == q)))
		{
			q = *p;
			Q = !Q;
		}
		p++;
	}
	if (Q)
		*r = ERROR;
	else
		*r = WORD;
	*dp = p;
}

char	get_token(char **s, char **st, char **et)
{
	char	*p;
	char	r;

	p = *s;
	while (*p && ft_strchr(WHITESPACES, *p))
		p++;
	if (st)
		*st = p;
	if (!*p)
		r = 0;
	else if (get_token_cmp2(p, &r))
		p += 2;
	else if (get_token_cmp1(p, &r))
		p += 1;
	else
		get_token_word(&p, &r);
	if (et)
		*et = p;
	*s = p;
	return (r);
}

char	token_peek(char *p)
{
	char	r;

	while (*p && ft_strchr(WHITESPACES, *p))
		p++;
	if (!*p)
		r = 0;
	else if (get_token_cmp2(p, &r))
		p += 2;
	else if (get_token_cmp1(p, &r))
		p += 1;
	else
		get_token_word(&p, &r);
	return (r);
}

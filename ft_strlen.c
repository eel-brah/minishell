
#include <stdlib.h>

int ft_strlen(char *s)
{
	int i; 

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*make_st()
{
	char	*s;

	s = malloc(6);
	s[0] = 'A';
	s[1] = 'h';
	s[2] = 'm';
	s[3] = 'e';
	s[4] = 'd';
	s[5] = '\0';
	return (s);
}
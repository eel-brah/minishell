
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

static char	*ft_itoa_fill(char *nbr, unsigned int n, int len, int sign)
{
	nbr[len] = '\0';
	while (len)
	{
		nbr[--len] = n % 10 + 48;
		n /= 10;
	}
	if (sign)
		nbr[len] = '-';
	return (nbr);
}

static int	ft_itoa_count(unsigned int nb)
{
	int	len;

	len = 0;
	while (nb)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	unsigned int	len;
	unsigned int	nb;
	char			*nbr;
	int				sign;

	sign = 0;
	nb = n;
	if (n < 0)
	{
		nb *= -1;
		sign = 1;
	}
	len = ft_itoa_count(nb) + sign;
	if (n == 0)
		len = 1;
	nbr = malloc(sizeof(char) * (len + 1));
	if (!nbr)
		return (NULL);
	return (ft_itoa_fill(nbr, nb, len, sign));
}

#include <unistd.h>
#include <stdio.h>

int main()
{
	if (isatty(STDIN_FILENO)) {
        printf("Standard input is a terminal: %s\n", ttyname(STDIN_FILENO));
    }
	else
		printf("not attty \n");
	return 0;
}

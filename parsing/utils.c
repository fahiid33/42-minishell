/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:06:43 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/18 04:20:24 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void switch_var(int *i)
{
	if(*i == 0)
		*i = 1;
	else
		*i = 0;
}

void	*f_malloc(size_t size)
{
	void	*tmp;

	tmp = malloc(size);
	if (!tmp)
		return (NULL);
	g_vars.alloc[g_vars.index] = tmp;
	g_vars.index++;
	return (tmp);
}

void	*realloc_array(char **arg, char *str)
{
	char	**new_arg;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (arg && arg[i])
		i++;
	new_arg = (char **)f_malloc(sizeof(char *) * (i + 2));
	while (arg && j < i)
	{
		new_arg[j] = ft_strdup(arg[j]);
		j++;
	}
	new_arg[j++] = ft_strdup(str);
	new_arg[j] = NULL;
	return (new_arg);
}

char	*ft_strdup(char *str)
{
	char	*dup;
	int		i;

	i = 0;
	dup = f_malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	str = f_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

int is_digit(char c)
{
	if(c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_token(char c)
{
	char	*tokens;
	int		j;

	tokens = "><| ";
	j = 0;
	while (tokens[j])
	{
		if (c == tokens[j])
			return (1);
		j++;
	}
	return (0);
}

char *ft_strndup(char *str, int i)
{
	char	*dup;
	int		j;

	j = 0;
	dup = f_malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	while (j < i)
	{
		dup[j] = str[j];
		j++;
	}
	dup[j] = '\0';
	return (dup);
}

int	counter(int n)
{
	int	count;

	count = 0;
	if (n < 0)
		n *= -1;
	while (n)
	{
			count++;
			n = n / 10;
	}
	return (count);
}

char	*minmax(int n, char *str)
{
	if (n == -2147483648)
	{
		str = ft_strdup("-2147483648");
		return (str);
	}
	return (NULL);
}

char	*negative(int *n, char *str, int count)
{
	*n = *n * -1;
	str = f_malloc(sizeof(char) * count + 2);
	if (str == NULL)
		return (0);
	str[0] = '-';
	return (str);
}
char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		count;

	str = NULL;
	i = 0;
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (minmax(n, str));
	count = counter(n);
	if (n < 0 && ++i)
		str = negative(&n, str, count);
	else
		str = f_malloc(sizeof(char) * count + 1);
	if (str == NULL)
		return (0);
	str[count + i] = 0;
	i = count - 1 + i;
	while (n != 0)
	{
		str[i] = n % 10 + 48;
		n = n / 10;
		i--;
	}
	return (str);
}

int	is_alpha(char c)
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	is_alphanum(char c)
{
	if(is_alpha(c) || is_digit(c))
		return (1);
	return (0);
}

int ft_strlen(char *str)
{
	int i = 0;

	while(str[i] != '\0')
		i++;
	return (i);
}

int	ft_int_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (-1);
	if (s[i] == '\0')
		return (-1);
	while (s[i])
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	if ((char)c == '\0')
		return (i);
	return (-1);
}

void	errors(int exit_status)
{
	if(exit_status == 2)
	{
		ft_putstr_fd("Minishell : unclosed quote!", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 2;
	}
	else if(exit_status == 258)
	{
		ft_putstr_fd("Minishell : syntax error near unexpected token", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 258;
	}
	else if (exit_status == 3)
	{
		ft_putstr_fd("Minishell : pipe open!", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 2;
	}
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (s)
	{
		while (s[i])
		{
			ft_putchar_fd(s[i], fd);
			i++;
		}
	}
}

int	only_enter(void)
{
	if (g_vars.line[0] == '\0' || ft_is_space())
	{
		free(g_vars.line);
		return (1);
	}
	return (0);
}

int	ft_is_space(void)
{
	int	i;

	i = 0;
	while (g_vars.line[i])
	{
		if (g_vars.line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	is_piped(void)
{
	int	i;

	i = 0;
	while (g_vars.line[i])
	{
		if (g_vars.line[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

void	free_unused(int frr, char *s1, char *s2)
{
	if (frr != -1)
	{
		if (frr == 0 && s1)
			free(s1);
		else if (frr == 1 && s2)
			free(s2);
		else if (frr == 2 && s1 && s2)
		{
			free(s1);
			free(s2);
		}
	}
}

void	join_string2(char *str, char *s2, int j)
{
	int	i;

	i = -1;
	while (s2[++i])
	{
		str[j] = s2[i];
		j++;
	}
	str[j] = '\0';
}

char	*ft_strjoino(char *s1, char *s2, int frr)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if ((!s1 && !s2))
		return (ft_strdup(""));
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	if ((!s1 && s2[0] == 0) || (s1[0] == 0 && !s2))
		return (ft_strdup(""));
	str = (char *)f_malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 2);
	if (str == 0)
		return (0);
	while (s1[i])
	{
		str[j++] = s1[i];
		i++;
	}
	join_string2(str, s2, j);
	free_unused(frr, s1, s2);
	return (str);
}
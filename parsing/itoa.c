/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 03:58:28 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 12:20:50 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*to_exp(char *to_expand, int flag)
{
	char	**exp;

	if (flag)
	{
		exp = ft_split(my_getenv(g_vars.my_env, to_expand), ' ');
		to_expand = _2d_arr_to_str(exp);
		return (to_expand);
	}
	else
	{
		to_expand = my_getenv(g_vars.my_env, to_expand);
	}
	return (to_expand);
}

char	*_2d_arr_to_str(char **arr)
{
	char	*str;
	int		i;

	i = 0;
	str = ft_strdup("");
	while (arr[i])
	{
		str = ft_strjoin(str, arr[i]);
		i++;
	}
	return (str);
}

static long	ft_count(int n)
{
	long	i;
	long	j;

	j = (long)n;
	i = 0;
	if (j < 0)
	{
		i++;
		j *= -1;
	}
	while (j > 0)
	{
		j = j / 10;
		i++;
	}
	return (i);
}

static void	ft_intochar(char *str, long *i, long *k, long *j)
{
	while (*k != 0)
	{
		if ((*k) > 0)
		{
			str[(*i) - 1] = (*j) % 10 + '0';
			(*i)--;
			(*k) = (*k) / 10;
			(*j) = (*j) / 10;
		}
		if (*k < 0)
		{
			str[*i] = (*j) % 10 + '0';
			(*i)--;
			(*k) = (*k) / 10;
			(*j) = (*j) / 10;
		}
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	long	i;
	long	k;
	long	j;

	j = (long)n;
	k = j;
	i = ft_count(j);
	if (j == 0)
		return (ft_strdup("0"));
	str = (char *)f_malloc(sizeof(char) * (i + 1));
	if (!str)
		return (0);
	if (n < 0)
		i--;
	if (j < 0)
	{
		j *= -1;
		str[0] = '-';
		str[i + 1] = 0;
	}
	else
		str[i] = 0;
	ft_intochar(str, &i, &k, &j);
	return (str);
}

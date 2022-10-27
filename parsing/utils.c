/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:06:43 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/26 02:02:22 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_alphanum(char c)
{
	if (is_alpha(c) || is_digit(c))
		return (1);
	return (0);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
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

void	errors2(int exit_status)
{
	if (exit_status == 3)
	{
		ft_putstr_fd("Minishell : pipe open!", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 2;
	}
	else if (exit_status == 4)
	{
		ft_putstr_fd("minishell : ambiguous redirect", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 1;
	}
}

int	expand_digit_helper(t_vars **p, char *value, int *i)
{
	(*p)->ss = expand_digit(value, &(*p)->result, i);
	if ((*p)->ss == 9)
	{
		*p = fill_sequences_adv(*p, 9, 1);
		return (1);
	}
	else
		*p = fill_sequences_adv(*p, (*p)->ss, 1);
	return (0);
}

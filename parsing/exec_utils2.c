/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 03:50:16 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 14:49:31 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	errors(int exit_status)
{
	if (exit_status == 2)
	{
		ft_putstr_fd("Minishell : unclosed quote!", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 2;
	}
	else if (exit_status == 258)
	{
		ft_putstr_fd("Minishell : syntax error near unexpected token", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 258;
	}
	errors2(exit_status);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

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

int	str_is_alphanum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_alphanum(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	only_enter(void)
{
	char	*str;

	if (g_vars.line[0] == '$')
	{
		if (str_is_alphanum(ft_substr(g_vars.line, 1, ft_strlen(g_vars.line))))
		{
			str = my_getenv(g_vars.my_env, ft_substr(g_vars.line, 1,
						ft_strlen(g_vars.line)));
			if (!str)
			{
				g_vars.exit_status = 0;
				free(g_vars.line);
				return (1);
			}
		}
	}
	if (g_vars.line[0] == '\0' || ft_is_space())
	{
		free(g_vars.line);
		return (1);
	}
	return (0);
}

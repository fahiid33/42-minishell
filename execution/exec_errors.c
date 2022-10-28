/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 02:12:33 by fstitou           #+#    #+#             */
/*   Updated: 2022/10/28 04:51:54 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	_errors(int err, char *str)
{
	if (err == 1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(" No such file or directory", 2);
		ft_putchar_fd('\n', 2);
		g_vars.exit_status = 1;
	}
}

void	save_address(char *add)
{
	g_vars.alloc[g_vars.index] = add;
	g_vars.index++;
}

int	only_space(char *str, char c)
{
	int	i;

	i = 0;
	if (!str && (c == 34 || c == 39))
		return (0);
	else if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

t_parse	*add_command(t_parse *commad)
{
	t_parse	*new;

	new = init_cmd();
	commad = add_command_back(commad, new);
	return (commad);
}
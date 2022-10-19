/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:33:00 by fahd              #+#    #+#             */
/*   Updated: 2022/10/10 02:29:16 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env(void)
{
	t_env	*env_print;

	env_print = g_vars.my_env;
	while (env_print)
	{
		if (env_print->sep)
		{
			ft_putstr_fd(env_print->key, 1);
			ft_putchar_fd(env_print->sep, 1);
			if (env_print->val == NULL)
				ft_putchar_fd('\n', 1);
			else
			{
				ft_putstr_fd(env_print->val, 1);
				ft_putchar_fd('\n', 1);
			}
		}
		env_print = env_print->next;
	}
	g_vars.exit_status = 0;
	return (g_vars.exit_status);
}

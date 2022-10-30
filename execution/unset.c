/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:32:07 by fahd              #+#    #+#             */
/*   Updated: 2022/10/30 20:25:59 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	str_digit(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	return (0);
}

int	check_arg(char *arg)
{
	if (str_is_alnum(arg) == 0 || (arg[0] >= '0' && arg[0] <= '9'))
	{
		ft_putstr_fd("unset: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument not allowed\n", 2);
		return (0);
	}
	return (1);
}

void	del_env_key(char *key, t_env **env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = *env;
	tmp2 = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (tmp == *env)
				*env = tmp->next;
			else
				tmp2->next = tmp->next;
			free(tmp->key);
			free(tmp->val);
			free(tmp);
			tmp = NULL;
			return ;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}
}

int	unset(t_parse *cmd)
{
	int		i;

	i = 0;
	while (cmd->argv[i])
	{
		if (check_arg(cmd->argv[i]))
			del_env_key(cmd->argv[i], &g_vars.my_env);
		else
		{
			g_vars.exit_status = 1;
			return (g_vars.exit_status);
		}
		i++;
	}
	g_vars.exit_status = 0;
	return (g_vars.exit_status);
}

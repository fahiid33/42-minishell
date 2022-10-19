/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export-env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:29:30 by aainhaja          #+#    #+#             */
/*   Updated: 2022/10/18 04:20:15 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_numb(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_digit(str[i]))
		{
			if (!is_piped())
				ft_putstr_fd("exit\n", 2);
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			g_vars.exit_status = 255;
			exit (g_vars.exit_status);
		}
		i++;
	}
}

char	*check_plus(char *key, int *k)
{
	if (key[ft_strlen(key) - 1] == '+')
	{
		*k = 1;
		key[ft_strlen(key) - 1] = 0;
	}
	return (key);
}

void	update_export(t_env **env, char *key, char sep, char *val)
{
	t_env	*tmp;
	int		k;

	tmp = (*env);
	k = 0;
	key = check_plus(key, &k);
	while (tmp)
	{
		if (!strcmp(key, tmp->key))
		{
			if (k)
			{
				tmp->sep = sep;
				tmp->val = ft_strjoino(tmp->val, val, 2);
				return ;
			}
			else
			{
				tmp->sep = sep;
				tmp->val = val;
				return ;
			}
		}
		tmp = tmp->next;
	}
}

int	str_is_alnum(char *str)
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

int	check_exp_arg(char *to_check)
{
	if (to_check && to_check[strlen(to_check) - 1] == '+'
		&& str_is_alnum(ft_substr(to_check, 0,
				strlen(to_check) - 1))
		&& my_getenv_key(&g_vars.my_env,
			ft_substr(to_check, 0, strlen(to_check) - 1)))
		to_check = ft_substr(to_check, 0, strlen(to_check) - 1);
	if (!to_check || str_is_alnum(to_check) == 0
		|| (to_check[0] >= '0' && to_check[0] <= '9'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(to_check, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		g_vars.exit_status = 1;
		return (0);
	}
	return (1);
}

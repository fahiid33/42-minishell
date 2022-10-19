/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export-env-utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:31:07 by aainhaja          #+#    #+#             */
/*   Updated: 2022/10/10 02:29:16 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../minishell.h"

t_env	*lst_new(char *key, char sep, char *val)
{
	t_env	*new;

	new = f_malloc(sizeof(t_env));
	new->key = key;
	new->val = val;
	new->sep = sep;
	new->next = NULL;
	return (new);
}

void	lst_add_backenv(t_env **lst, t_env *new)
{
	t_env	*tmp;

	tmp = *lst;
	if (!new)
		return ;
	new->next = NULL;
	if (!*lst)
		*lst = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	init_env(char **env)
{
	char	*key;
	char	*val;
	int		i;
	char	**tmp;

	i = 0;
	while (env[i])
	{
		tmp = ft_split(env[i], '=');
		key = tmp[0];
		val = tmp[1];
		lst_add_backenv(&g_vars.my_env, lst_new(key, '=',
				val));
		i++;
	}
	lst_add_backenv(&g_vars.my_env, lst_new("0", '=', "minishell"));
}

char	*my_getenv(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = (env);
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
			return (tmp->val);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*my_getenv_key(t_env **env, char *key)
{
	t_env	*tmp;

	tmp = (*env);
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
			return (tmp->key);
		tmp = tmp->next;
	}
	return (NULL);
}

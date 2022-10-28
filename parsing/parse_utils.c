/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 03:22:18 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 14:08:19 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_vars	*fill_sequences_adv(t_vars *p, int len, int x)
{
	while (len--)
	{
		p->seq[p->o] = x;
		(p->o)++;
	}
	return (p);
}

int	expand_digit(char *value, char **result, int *i)
{
	int	len;

	len = 0;
	if (value[*i] && value[*i] == '0')
	{
		*result = ft_strjoin(*result, my_getenv(g_vars.my_env, "0"));
		(*i)++;
		return (9);
	}
	(*i)++;
	while (value[*i] && !is_token(value[*i]) && value[*i] != '$'
		&& value[*i] != 34)
	{
		*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
		(*i)++;
		len++;
	}
	(*i)--;
	return (len);
}

int	simple_expand(char *value, char **result, int *i, int flag)
{
	char	*to_expand;
	int		len;

	to_expand = ft_strdup("");
	len = 0;
	while (is_alphanum(value[*i]))
	{
		to_expand = ft_strjoin(to_expand, ft_strndup(&value[*i], 1));
		(*i)++;
	}
	if (my_getenv(g_vars.my_env, to_expand))
	{
		to_expand = to_exp(to_expand, flag);
		*result = ft_strjoin(*result, to_expand);
		len = ft_strlen(my_getenv(g_vars.my_env, to_expand));
	}
	else
		*result = ft_strjoin(*result, ft_strdup(""));
	(*i)--;
	return (len);
}

int	expand_in_dq(char *value, char **result, int *i, int f)
{
	char	*dq;

	dq = ft_strdup("");
	(*i)++;
	if (ft_int_strchr(&value[*i], 34) == -1 && !f)
	{
		errors(2);
		return (-1);
	}
	else if (value[*i] && value[*i] == 34)
	{
		(*i)++;
		return (0);
	}
	while (value[*i] && value[*i] != 34)
	{
		dq = ft_strjoin(dq, ft_strndup(&value[*i], 1));
		(*i)++;
	}
	*result = ft_strjoin(*result, dq_content(dq));
	if (ft_strcmp(dq_content(dq), ft_strdup("")) == 0)
		(*i)++;
	return (ft_strlen(dq_content(dq)));
}

int	collect_squote(char *value, char **result, int *i, int f)
{
	int	len;

	len = 0;
	*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
	(*i)++;
	if (ft_int_strchr(&value[*i], 39) == -1 && !f)
	{
		errors(2);
		return (-1);
	}
	len++;
	while (value[*i] && value[*i] != 39)
	{
		*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
		(*i)++;
		len++;
	}
	if (value[*i])
	{
		*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
		len++;
	}
	return (len);
}

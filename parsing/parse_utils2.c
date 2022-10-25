/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 03:32:57 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/25 05:08:03 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ambiguous_redirect(t_token **tokens)
{
	char	*str;
	t_vars	*p;

	p = init_vars();
	str = ft_strdup("");
	(*tokens) = (*tokens)->next;
	if (!(*tokens)->value[1])
		return ;
	str = ft_substr((*tokens)->value, 1, ft_strlen((*tokens)->value));
	if (my_getenv(g_vars.my_env, str) || str[0] == 34 || str[0] == 39)
	{
		(*tokens)->value = expand_word((*tokens)->value, p, 0);
		(*tokens)->value = remove_quotes((*tokens)->value, p->seq);
		return ;
	}
	(*tokens)->value = ft_strdup("*");
}

int	parse_delimiter(t_token **tokens, t_vars *p)
{
	*tokens = (*tokens)->next;
	fill_sequences(ft_strlen((*tokens)->value), p);
	(*tokens)->value = remove_quotes((*tokens)->value, p->seq);
	if ((*tokens)->value == NULL)
		return (1);
	return (0);
}

int	collect_inside_quotes(char *value, int *i, char **result)
{
	char	q;

	q = value[*i];
	(*i)++;
	if (q == 39 || q == 34)
	{
		if ((value[*i] && ft_int_strchr(&value[*i], q) == -1
				&& g_vars.g_err != 1) || value[*i] == '\0')
		{
			errors(2);
			return (1);
		}
	}
	if (g_vars.g_err != 1)
	{
		while (value[*i] && value[*i] != q)
		{
			*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
			(*i)++;
		}
	}
	(*i)++;
	return (0);
}

void	collect_expanded(char *value, char **result, int *i)
{
	if (g_vars.g_err != 1)
		*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
	(*i)++;
}

int	expand_dq(char *value, char **result, int *i)
{
	(*i)++;
	if (is_digit(value[*i]))
	{
		if (expand_digit(value, &(*result), &(*i)))
			return (1);
	}
	else if (value[*i] == '$')
		(*result) = ft_strjoin((*result), ft_itoa(g_vars.pid));
	else if (value[*i] == '?')
		(*result) = ft_strjoin((*result), ft_itoa(g_vars.exit_status));
	else if ((value[*i] == 39 || value[*i] == 34))
	{
		(*result) = ft_strjoin((*result), ft_strndup(&value[*i], 1));
		(*i)++;
		return (1);
	}
	else if ((value[*i] && !is_alpha(value[*i]) && !is_digit(value[*i])))
	{
		(*result) = ft_strjoin((*result), "$");
		return (1);
	}
	else
		simple_expand(value, &(*result), &(*i));
	return (0);
}

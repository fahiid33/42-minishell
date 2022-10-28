/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 03:34:20 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 10:19:12 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	collect_char(char *value, char **result, int i)
{
	*result = ft_strjoin(*result, ft_strndup(&value[i], 1));
	return (1);
}

int	expand_dd(char **result)
{
	int	len;

	len = ft_strlen(ft_itoa(g_vars.pid));
	*result = ft_strjoin(*result, ft_itoa(g_vars.pid));
	return (len);
}

int	collect_dollar(char **result)
{
	*result = ft_strjoin(*result, ft_strdup("$"));
	return (1);
}

int	get_exit_status(char **result)
{
	int	len;

	len = ft_strlen(ft_itoa(g_vars.exit_status));
	*result = ft_strjoin(*result, ft_itoa(g_vars.exit_status));
	return (len);
}

int	parse_word(t_token **tokens, t_vars *p)
{
	if (there_is_dollar((*tokens)->value))
	{
		(*tokens)->value = expand_word((*tokens)->value, p, 0);
		if ((*tokens)->value == NULL)
			return (1);
	}
	else
	{
		fill_sequences(ft_strlen((*tokens)->value), p);
	}
	(*tokens)->value = remove_quotes(ft_strdup((*tokens)->value), p->seq);
	if ((*tokens)->value == NULL)
		return (1);
	return (0);
}

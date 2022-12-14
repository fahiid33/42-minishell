/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:17:49 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 14:16:54 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*parsing(t_token *tokens)
{
	t_token	*tmp;
	t_vars	*p;
	t_token	*prev;

	tmp = tokens->next;
	prev = tokens;
	tokens = tokens->next;
	while (tokens->e_type != END)
	{
		p = init_vars();
		if (parsing_helper(&tokens, prev, p))
			return (NULL);
		prev = tokens;
		tokens = tokens->next;
	}
	return (tmp);
}

int	there_is_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_word(char *value, t_vars *p, int f)
{
	int		i;
	int		check;

	i = 0;
	while (value[i] != '\0')
	{
		check = expand_word_norm(value, &i, &p, f);
		if (check == 1)
			break ;
		else if (check == 2)
			continue ;
		else if (check == 3)
			return (NULL);
		if (!value[i])
			break ;
		i++;
	}
	return (p->result);
}

char	*remove_quotes(char *value, int *sequences)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (value[i] != '\0')
	{
		if (sequences[i] != 1)
		{
			if (value[i] == 34 || value[i] == 39)
			{
				if (collect_inside_quotes(value, &i, &result))
					return (NULL);
			}
			if (g_vars.g_err != 1 && value[i] != '\0' && value[i] != 34
				&& value[i] != 39)
			{
				result = ft_strjoin(result, ft_strndup(&value[i], 1));
				i++;
			}
		}
		else
			collect_expanded(value, &result, &i);
	}
	return (result);
}

char	*dq_content(char *value)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1]
			&& (is_alphanum(value[i + 1]) || value[i + 1] == '?'
				|| value[i + 1] == '$'))
		{
			if (expand_dq(value, &result, &i))
			{
				continue ;
			}
		}
		else
			result = ft_strjoin(result, ft_strndup(&value[i], 1));
		i++;
	}
	return (result);
}

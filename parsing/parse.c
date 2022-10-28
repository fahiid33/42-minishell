/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:17:49 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 08:51:52 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*parsing(t_token *tokens)
{
	t_token	*tmp;
	t_vars	*p;
	t_token  *prev;

	p = init_vars();
	tmp = tokens->next;
	prev = tokens;
	tokens = tokens->next;
	while (tokens->e_type != END)
	{
		if (tokens->e_type == WORD)
		{
			if (tokens->value[0] == '$' && (prev->value == NULL || prev->e_type == PIPE))
				g_vars.flag++;
			if (parse_word(&tokens, p))
				return (NULL);
		}
		if ((tokens->e_type == GREATANDGREAT || tokens->e_type == GREAT
				|| tokens->e_type == LESS)
			&& tokens->next && tokens->next->value[0] == '$')
			ambiguous_redirect(&tokens);
		else if (tokens->e_type == LESSANDLESS && tokens->next->e_type == WORD)
		{
			if (parse_delimiter(&tokens, p))
				return (NULL);
		}
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

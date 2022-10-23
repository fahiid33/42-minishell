/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:17:49 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/23 02:04:58 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("token: %s, type: %d\n", tokens->value, tokens->e_type);
		tokens = tokens->next;
	}
}

t_token	*parsing(t_token *tokens)
{
	int		*sequences;
	t_token	*tmp;

	tmp = tokens->next;
	tokens = tokens->next;
	sequences = f_malloc(sizeof(int) * 10000);
	while (tokens->e_type != END)
	{
		if (tokens->e_type == WORD)
		{
			if (parse_word(&tokens, &sequences))
				return (NULL);
		}
		if ((tokens->e_type == GREATANDGREAT || tokens->e_type == GREAT
				|| tokens->e_type == LESS)
			&& tokens->next && tokens->next->value[0] == '$')
			ambiguous_redirect(&tokens);
		else if (tokens->e_type == LESSANDLESS && tokens->next->e_type == WORD)
		{
			if (parse_delimiter(&tokens, &sequences))
				return (NULL);
		}
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

void	print_int_tab(int *tab)
{
	int	i;

	i = 0;
	while (tab[i] == 2 || tab[i] == 1)
	{
		printf("%d", tab[i]);
		i++;
	}
	printf("\n");
}

char	*expand_dollar(char *value, int *sequences, int f)
{
	int		i;
	int		o;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	o = 0;
	result = ft_strdup("");
	while (value[i] != '\0')
	{
		if (value[i] == '$')
		{
			i++;
			if (value[i] == '\0')
			{
				fill_sequences_adv(sequences, &o, collect_dollar(&result), 1);
				break ;
			}
			else if (is_digit(value[i]))
			{
				if (expand_digit(value, &result, &i))
					continue ;
			}
			else if (value[i] == '$')
				fill_sequences_adv(sequences, &o, expand_dd(&result), 1);
			else if (value[i] == '?')
				fill_sequences_adv(sequences, &o, get_exit_status(&result), 1);
			else if ((value[i] == 39 || value[i] == 34))
				continue ;
			else if ((value[i] && !is_alpha(value[i]) && !is_digit(value[i])))
			{
				fill_sequences_adv(sequences, &o, collect_dollar(&result), 1);
				continue ;
			}
			else
				fill_sequences_adv(sequences, &o, simple_expand(value, &result, &i), 1);
		}
		else if (value[i] == 34)
		{
			i++;
			int ok = expand_in_dq(value, &result, &i, f);
			if(ok == -1)
				return(NULL);
			else if(ok == 0)
				continue ;
			else
				fill_sequences_adv(sequences, &o, ok, 1);
		}
		else if (value[i] && value[i] == 39)
		{
			int ok = collect_squote(value, &result, &i, f);
			if(ok == -1)
				return(NULL);
			fill_sequences_adv(sequences, &o, ok, 2);
		}
		else
			fill_sequences_adv(sequences, &o, collect_char(value, &result, i), 1);
		if (!value[i])
			break ;
		i++;
	}
	return (result);
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

void	fill_sequences(int len, int *sequences)
{
	int	i;

	i = 0;
	while (i < len)
	{
		sequences[i] = 2;
		i++;
	}
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
			if(expand_dq(value, &result, &i))
				continue ;
		}
		else
			result = ft_strjoin(result, ft_strndup(&value[i], 1));
		i++;
	}
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 00:29:36 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/25 04:09:02 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_vars	*init_vars(void)
{
	t_vars	*p;

	p = f_malloc(sizeof(t_vars));
	p->o = 0;
	p->result = ft_strdup("");
	p->seq = f_malloc(sizeof(int) * 10000);
	return (p);
}

int	expand_dollar(char *value, int *i, t_vars **p)
{
	(*i)++;
	if (value[*i] == '\0')
	{
		*p = fill_sequences_adv(*p, collect_dollar(&(*p)->result), 1);
		return (2);
	}
	else if (is_digit(value[*i]))
	{
		int ss = expand_digit(value, &(*p)->result, i);
		if (ss == 9)
		{
			*p = fill_sequences_adv(*p, 9, 1);
			return (1);
		}
		else
			*p = fill_sequences_adv(*p, ss, 1);
	}
	else if (value[*i] == '$')
		*p = fill_sequences_adv(*p, expand_dd(&(*p)->result), 1);
	else if (value[*i] == '?')
		*p = fill_sequences_adv(*p, get_exit_status(&(*p)->result), 1);
	else if ((value[*i] == 39 || value[*i] == 34))
		return (1);
	else if ((value[*i] && !is_alpha(value[*i]) && !is_digit(value[*i])))
	{
		*p = fill_sequences_adv(*p, collect_dollar(&(*p)->result), 1);
		return (1);
	}
	else
		*p = fill_sequences_adv(*p, simple_expand(value, &(*p)->result, i), 1);
	return (0);
}

int	collect_sq_check(char *value, int *i, t_vars **p, int f)
{
	int	check;

	check = collect_squote(value, &(*p)->result, i, f);
	if (check == -1)
		return (-1);
	*p = fill_sequences_adv(*p, check, 2);
	return (0);
}

int	collect_dq_check(char *value, int *i, t_vars **p, int f)
{
	int	check;

	check = expand_in_dq(value, &(*p)->result, i, f);
	if (check == -1)
		return (-1);
	else if (check == 0)
		return (1);
	else
		*p = fill_sequences_adv(*p, check, 1);
	return (0);
}

int	expand_word_norm(char *value, int *i, t_vars **p, int f)
{
	if (value[*i] == '$')
	{
		(*p)->check = expand_dollar(value, i, p);
		if ((*p)->check == 2)
			return (1);
		else if ((*p)->check == 1)
			return (2);
	}
	else if (value[*i] == 34)
	{
		(*p)->check = collect_dq_check(value, i, p, f);
		if ((*p)->check == -1)
			return (3);
		else if ((*p)->check == 1)
			return (2);
	}
	else if (value[*i] && value[*i] == 39)
	{
		if (collect_sq_check(value, i, p, f) == -1)
			return (3);
	}
	else
		*p = fill_sequences_adv(*p, collect_char(value, &(*p)->result, *i), 1);
	return (0);
}

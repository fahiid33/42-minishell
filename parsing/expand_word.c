/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 00:29:36 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 04:53:31 by amoubare         ###   ########.fr       */
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
	p->ss = 0;
	return (p);
}

int	expand_dollar(char *val, int *i, t_vars **p)
{
	(*i)++;
	if (val[*i] == '\0')
	{
		*p = fill_sequences_adv(*p, collect_dollar(&(*p)->result), 1);
		return (2);
	}
	else if (is_digit(val[*i]))
	{
		if (expand_digit_helper(p, val, i))
			return (1);
	}
	else if (val[*i] == '$')
		*p = fill_sequences_adv(*p, expand_dd(&(*p)->result), 1);
	else if (val[*i] == '?')
		*p = fill_sequences_adv(*p, get_exit_status(&(*p)->result), 1);
	else if ((val[*i] == 39 || val[*i] == 34))
		return (1);
	else if ((val[*i] && !is_alpha(val[*i]) && !is_digit(val[*i])))
	{
		*p = fill_sequences_adv(*p, collect_dollar(&(*p)->result), 1);
		return (1);
	}
	else
		*p = fill_sequences_adv(*p, simple_expand(val, &(*p)->result, i, 1), 1);
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

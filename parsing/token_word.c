/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 23:11:17 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/23 03:35:53 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	advance_lex(t_lex *lex)
{
	if (lex->cmd[lex->i])
	{
			lex->i++;
			lex->c = lex->cmd[lex->i];
	}	
}

void	collect_to_token(int *i, int *j, t_lex **lex, char **val)
{
	while ((*lex)->c && !is_token((*lex)->c) && (*lex)->c != '\0')
	{
		if ((*lex)->c == 34)
			switch_var(i);
		else if ((*lex)->c == '\'')
			switch_var(j);
		*val = ft_strjoin(*val, ft_strndup(&(*lex)->c, 1));
		advance_lex(*lex);
		if (*i == 1 || *j == 1)
			break ;
	}
}

void	continue_to_dquote(int *i, t_lex **lex, char **val)
{
	while ((*lex)->c != 34 && (*lex)->c != '\0')
	{
		*val = ft_strjoin(*val, ft_strndup(&(*lex)->c, 1));
		advance_lex(*lex);
	}
	*val = ft_strjoin(*val, ft_strndup(&(*lex)->c, 1));
	advance_lex(*lex);
	switch_var(i);
}

void	continue_to_squote(int *j, t_lex **lex, char **val)
{
	while ((*lex)->c && (*lex)->c != '\'' && (*lex)->c != '\0')
	{
		*val = ft_strjoin(*val, ft_strndup(&(*lex)->c, 1));
		advance_lex(*lex);
	}
	*val = ft_strjoin(*val, ft_strndup(&(*lex)->c, 1));
	advance_lex(*lex);
	switch_var(j);
}

int	collect_to_token_repeat(int *i, int *j, t_lex **lex, char **val)
{
	while ((*lex)->c && (*lex)->c != '>' && (*lex)->c != '<' && (*lex)->c != '|'
		&& (*lex)->c != ' ' && (*lex)->c != '\0')
	{
		if ((*lex)->c == 34)
			switch_var(i);
		else if ((*lex)->c == '\'')
			switch_var(j);
		*val = ft_strjoin(*val, ft_strndup(&(*lex)->c, 1));
		advance_lex((*lex));
		if (*i == 1 || *j == 1)
		{
			return (1);
		}	
	}
	return (0);
}

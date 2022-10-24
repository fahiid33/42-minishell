/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:09:30 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/23 03:41:03 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	token_pipe(t_lex *lex, t_token *tokens)
{
	char	*val;
	t_token	*new;

	val = ft_strdup("|");
	new = NULL;
	advance_lex(lex);
	new = init_token(val, PIPE);
	tokens = lst_add_back(tokens, new);
}

void	token_great(t_lex *lex, t_token *tokens)
{
	char	*val;
	t_token	*new;

	val = ft_strdup("");
	new = NULL;
	advance_lex(lex);
	if (lex->cmd[lex->i] == '>')
	{
		advance_lex(lex);
		val = ft_strdup(">>");
		new = init_token(val, GREATANDGREAT);
	}
	else
	{
		val = ft_strdup(">");
		new = init_token(val, GREAT);
	}
	tokens = lst_add_back(tokens, new);
}

void	token_less(t_lex *lex, t_token *tokens)
{
	char	*val;
	t_token	*new;

	val = ft_strdup("");
	new = NULL;
	advance_lex(lex);
	if (lex->cmd[lex->i] == '<')
	{
		advance_lex(lex);
		val = ft_strdup("<<");
		new = init_token(val, LESSANDLESS);
	}
	else
	{
		val = ft_strdup("<");
		new = init_token(val, LESS);
	}
	tokens = lst_add_back(tokens, new);
}

void	token_word(t_lex *lex, t_token *tokens)
{
	char	*val;
	t_token	*new;
	int		i;
	int		j;
	int		k;

	val = ft_strdup("");
	new = NULL;
	i = 0;
	j = 0;
	k = 1;
	while (lex->c != '\0' && k == 1)
	{
		k = 0;
		collect_to_token(&i, &j, &lex, &val);
		if (i == 1)
			continue_to_dquote(&i, &lex, &val);
		else if (j == 1)
			continue_to_squote(&j, &lex, &val);
		k = collect_to_token_repeat(&i, &j, &lex, &val);
	}
	new = init_token(val, WORD);
	tokens = lst_add_back(tokens, new);
}

void	end_token(t_token *tokens)
{
	t_token	*new;

	new = NULL;
	new = init_token("", END);
	tokens = lst_add_back(tokens, new);
}

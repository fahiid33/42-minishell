/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 03:41:24 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/24 00:28:11 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_sequences(int len, t_vars	*p)
{
	int	i;

	i = 0;
	while (i < len)
	{
		p->seq[i] = 2;
		i++;
	}
}

t_token	*lst_add_back(t_token *lst, t_token *new)
{
	t_token	*tmp;

	if (!lst)
		return (new);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (lst);
}

t_token	*init_create_tokens(t_token *tokens, char *line)
{
	t_lex	*lex;

	lex = f_malloc(sizeof(t_lex));
	tokens = init_token(NULL, 0);
	if (!lex || !tokens)
		return (NULL);
	lex->c = line[0];
	lex->cmd = line;
	lex->i = 0;
	tokens = create_tokens(lex, tokens);
	return (tokens);
}

t_token	*init_token(char *val, int type)
{
	t_token	*token;

	token = (t_token *)f_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = val;
	token->next = NULL;
	token->e_type = type;
	return (token);
}

t_token	*create_tokens(t_lex *lex, t_token *tokens)
{
	while (lex->c)
	{
		if (lex->c == 32 || lex-> c == 9)
			advance_lex(lex);
		else if (lex->c == '|')
			token_pipe(lex, tokens);
		else if (lex->c == '>')
			token_great(lex, tokens);
		else if (lex->c == '<')
			token_less(lex, tokens);
		else
			token_word(lex, tokens);
	}
	end_token(tokens);
	return (tokens);
}

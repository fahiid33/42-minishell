/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:09:30 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/19 00:46:47 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	while(lex->c)
	{
		if(lex->c == 32 || lex-> c == 9)
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
	return(tokens);
}

void	token_pipe(t_lex *lex, t_token *tokens)
{
	char *val;
	t_token *new;

	val = ft_strdup("|");
	new = NULL;
	advance_lex(lex);
	new = init_token(val, PIPE);
	tokens = lst_add_back(tokens, new);
}

void	token_great(t_lex *lex, t_token *tokens)
{
	char *val;
	t_token *new;

	val = ft_strdup("");
	new = NULL;
	advance_lex(lex);
	if	(lex->cmd[lex->i] == '>')
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
	char *val;
	t_token *new;

	val = ft_strdup("");
	new = NULL;
	advance_lex(lex);
	if	(lex->cmd[lex->i] == '<')
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
	char *val;
	t_token *new;
	int i = 0;
	int j = 0;
	int k = 1;

	val = ft_strdup("");
	new = NULL;
	while(lex->c != '\0' && k == 1)
	{
		k = 0;
		while (lex->c && !is_token(lex->c) && lex->c != '\0')
		{
			if(lex->c == 34)
				switch_var(&i);
			else if(lex->c == '\'')
				switch_var(&j);
			val = ft_strjoin(val, ft_strndup(&lex->c, 1));
			advance_lex(lex);
			if(i == 1 || j == 1)
				break;
		}
		if(i == 1)
		{
			while(lex->c != 34 && lex->c != '\0')
			{
				val = ft_strjoin(val, ft_strndup(&lex->c, 1));
				advance_lex(lex);
			}
			val = ft_strjoin(val, ft_strndup(&lex->c, 1));
			advance_lex(lex);
			switch_var(&i);
		}
		else if(j == 1)
		{
			while(lex->c && lex->c != '\'' && lex->c != '\0')
			{
				val = ft_strjoin(val, ft_strndup(&lex->c, 1));
				advance_lex(lex);
			}
			val = ft_strjoin(val, ft_strndup(&lex->c, 1));
			advance_lex(lex);
			switch_var(&j);
		}
		while(lex->c && lex->c != '>' && lex->c != '<' && lex->c != '|'
			&& lex->c != ' ' && lex->c != '\0')
		{
			if(lex->c == 34)
				switch_var(&i);
			else if(lex->c == '\'')
				switch_var(&j);
			val = ft_strjoin(val, ft_strndup(&lex->c, 1));
			advance_lex(lex);
			if(i == 1 || j == 1)
			{
				k=1;
				break;
			}	
		}
	}
	new = init_token(val, WORD);
	tokens = lst_add_back(tokens, new);
}

void	end_token(t_token *tokens)
{
	t_token *new;

	new = NULL;
	new = init_token("", END);
	tokens = lst_add_back(tokens, new);
}
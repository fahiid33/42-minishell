/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 23:20:04 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/26 00:27:21 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_tparse(t_token *tokens, t_parse **parse)
{
	t_parse	*tmp;

	tmp = *parse;
	while (tokens)
	{
		if (tokens->e_type == WORD)
			fill_cmd_args(&tokens, &tmp);
		else if (tokens->e_type == GREAT || tokens->e_type == LESS
			|| tokens->e_type == LESSANDLESS || tokens->e_type == GREATANDGREAT)
		{
			if (fill_redir_file(&tokens, &tmp))
				return ;
		}
		else if (tokens->e_type == PIPE || tokens->e_type == END)
		{
			if (fill_cmd(&tokens, &tmp))
				return ;
		}
	}
}

void	fill_cmd_args(t_token **tokens, t_parse **tmp)
{
	if (!(*tmp)->cmd)
		(*tmp)->cmd = ft_strdup((*tokens)->value);
	else
		(*tmp)->argv = realloc_array((*tmp)->argv, (*tokens)->value);
	(*tokens) = (*tokens)->next;
}

int	fill_redir_file(t_token **tokens, t_parse **tmp)
{
	int	type;

	if ((*tokens)->next->e_type != WORD)
	{
		errors(258);
		return (1);
	}
	type = (*tokens)->e_type;
	(*tokens) = (*tokens)->next;
	if (!(*tmp)->redir)
		(*tmp)->redir = init_redir((*tokens)->value, type);
	else
		(*tmp)->redir = add_redir((*tmp)->redir, (*tokens)->value, type);
	(*tokens) = (*tokens)->next;
	return (0);
}

int	fill_cmd(t_token **tokens, t_parse **tmp)
{
	if ((*tokens)->next && (*tokens)->e_type == PIPE
		&& (*tokens)->next->e_type == END)
	{
		errors(3);
		return (1);
	}
	*tmp = add_command(*tmp);
	*tmp = (*tmp)->next;
	(*tokens) = (*tokens)->next;
	return (0);
}

t_parse	*add_command(t_parse *commad)
{
	t_parse	*new;

	new = init_cmd();
	commad = add_command_back(commad, new);
	return (commad);
}

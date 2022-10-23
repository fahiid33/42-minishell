/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 23:20:04 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/22 23:34:57 by amoubare         ###   ########.fr       */
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
			if(fill_redir_file(&tokens, &tmp))
					return ;
		}
		else if (tokens->e_type == PIPE || tokens->e_type == END)
		{
			if (tokens->next && tokens->e_type == PIPE
				&& tokens->next->e_type == END)
			{
				errors(3);
				return ;
			}
			tmp = add_command(tmp);
			tmp = tmp->next;
			tokens = tokens->next;
		}
	}
}

void    fill_cmd_args(t_token **tokens, t_parse **tmp)
{
    if (!(*tmp)->cmd)
        (*tmp)->cmd = ft_strdup((*tokens)->value);
    else
        (*tmp)->argv = realloc_array((*tmp)->argv, (*tokens)->value);
    (*tokens) = (*tokens)->next;
}

int    fill_redir_file(t_token **tokens, t_parse **tmp)
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
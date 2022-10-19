/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:05:49 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/18 04:05:21 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_parse	*init_cmd(void)
{
	t_parse	*command;

	command = (t_parse *)f_malloc(sizeof(t_parse));
	if (!command)
		return (NULL);
	command->cmd = NULL;
	command->argv = (char **)realloc_array(NULL, ft_strdup(""));
	command->redir = NULL;
	command->next = NULL;
	return (command);
}

t_parse	*add_command_back(t_parse *lst, t_parse *new)
{
	t_parse	*tmp;

	tmp = lst;
	if (!lst)
		return (new);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (lst);
}

t_redir	*init_redir(char *val, int type)
{
	t_redir	*redir;

	redir = (t_redir *)f_malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(val);
	redir->next = NULL;
	redir->e_type = type;
	return (redir);
}

t_redir	*lst_add_back_redir(t_redir *lst, t_redir *new)
{
	t_redir	*tmp;

	if (!lst)
		return (new);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (lst);
}

t_redir	*add_redir(t_redir *redir, char *val, int type)
{
	t_redir	*new;

	new = init_redir(val, type);
	redir = lst_add_back_redir(redir, new);
	return (redir);
}

t_parse	*add_command(t_parse *commad)
{
	t_parse	*new;

	new = init_cmd();
	commad = add_command_back(commad, new);
	return (commad);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 23:20:04 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/28 09:04:11 by amoubare         ###   ########.fr       */
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
		{
			// printf ("%s\n",tokens->value);
			fill_cmd_args(&tokens, &tmp);
		}
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

int	there_is_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strjoin_args(char **sp, int i)
{
	int		j;
	char	*ret;

	ret = ft_strdup("");
	j  = 1;
	while (j <= i)
	{
		ret = ft_strjoino(ret, sp[j], -1);
		if (j != i)
			ret = ft_strjoino(ret, " ", -1);
		j++;
	}
	return (ret);
}

char	*split_args(char *str, int flag)
{
	char	**sp;
	int		i;

	i = 0;
	sp = ft_split(str, ' ');
	while (sp[i])
		i++;
	if (!sp)
		return (NULL);
	if (flag == 1)
		str = sp[0];
	else
		str = ft_strjoin_args(sp, i);
	return (str);
}

void	print_tparse(t_parse *parse)
{
	t_parse	*tmp;
	t_redir	*tmp2;

	tmp = parse;
	while (tmp)
	{
		printf("cmd: %s", tmp->cmd);
		if (tmp->argv)
		{
			printf(" argv: ");
			int i = 0;
			while (tmp->argv[i])
			{
				printf(" %s\n", tmp->argv[i]);
				i++;
			}
		}
		tmp = tmp->next;
	}
}

int	check_token(void)
{
	int	i;

	i = 1;
	if (g_vars.line[0] == '$')
	{
		while (g_vars.line[i])
		{
			if (!is_alpha(g_vars.line[i]))
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

int str_is_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

void	fill_cmd_args(t_token **tokens, t_parse **tmp)
{
	// if (g_vars.flag && !(*tmp)->cmd && str_is_space((*tokens)->value))
	// {
	// 	int		i;
	// 	char	*cmd;
	// 	char	*args;
	// 	char	**argv;
	
	// 	g_vars.flag--;
	// 	i = 0;
	// 	// printf("value: %s\n",(*tokens)->value);
	// 	cmd = split_args((*tokens)->value, 1);
	// 	// printf("cmd: %s   ",cmd);
	// 	args = split_args((*tokens)->value, 0);
	// 	// printf("args: %s\n",args);
	// 	argv = ft_split(args, ' ');
	// 	if (!(*tmp)->cmd)
	// 	{
	// 		(*tmp)->cmd = cmd;
	// 		while (argv[i])
	// 		{
	// 			(*tmp)->argv = realloc_array((*tmp)->argv,
	// 					ft_strdup(argv[i++]));
	// 		}
	// 	}
	// 	(*tokens) = (*tokens)->next;
	// }
	// else 
	// {
		if (!(*tmp)->cmd)
			(*tmp)->cmd = ft_strdup((*tokens)->value);
		else
			(*tmp)->argv = realloc_array((*tmp)->argv, (*tokens)->value);
		(*tokens) = (*tokens)->next;
	// }
	// print_tparse(*tmp);
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

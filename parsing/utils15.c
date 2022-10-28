/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils15.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:59:29 by fstitou           #+#    #+#             */
/*   Updated: 2022/10/28 14:20:43 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	j = 1;
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

int	ft_is_space(void)
{
	int	i;

	i = 0;
	while (g_vars.line[i])
	{
		if (g_vars.line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	parsing_helper(t_token **tokens, t_token *prev, t_vars *p)
{
	if ((*tokens)->e_type == WORD)
	{
		if ((*tokens)->value[0] == '$'
			&& (prev->value == NULL || prev->e_type == PIPE))
			g_vars.flag++;
		if (parse_word(tokens, p))
			return (1);
	}
	if (((*tokens)->e_type == GREATANDGREAT || (*tokens)->e_type == GREAT
			|| (*tokens)->e_type == LESS)
		&& (*tokens)->next && (*tokens)->next->value[0] == '$')
		ambiguous_redirect(tokens);
	else if ((*tokens)->e_type == LESSANDLESS
		&& (*tokens)->next->e_type == WORD)
	{
		if (parse_delimiter(tokens, p))
			return (1);
	}
	return (0);
}

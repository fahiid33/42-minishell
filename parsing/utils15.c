/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils15.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:59:29 by fstitou           #+#    #+#             */
/*   Updated: 2022/10/28 10:59:44 by fstitou          ###   ########.fr       */
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

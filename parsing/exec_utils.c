/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 03:43:22 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/31 00:04:20 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1)
		return (-1);
	while (s1 && s2 && s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	is_piped(void)
{
	int	i;

	i = 0;
	while (g_vars.line[i])
	{
		if (g_vars.line[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

void	free_unused(int frr, char *s1, char *s2)
{
	if (frr != -1)
	{
		if (frr == 0 && s1)
			free(s1);
		else if (frr == 1 && s2)
			free(s2);
		else if (frr == 2 && s1 && s2)
		{
			free(s1);
			free(s2);
		}
	}
}

void	join_string2(char *str, char *s2, int j)
{
	int	i;

	i = -1;
	while (s2[++i])
	{
		str[j] = s2[i];
		j++;
	}
	str[j] = '\0';
}

char	*ft_strjoino(char *s1, char *s2, int frr)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if ((!s1 && !s2))
		return (ft_strdup(""));
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	if ((!s1 && s2[0] == 0) || (s1[0] == 0 && !s2))
		return (ft_strdup(""));
	str = (char *)f_malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 2);
	if (str == 0)
		return (0);
	while (s1[i])
	{
		str[j++] = s1[i];
		i++;
	}
	join_string2(str, s2, j);
	free_unused(frr, s1, s2);
	return (str);
}

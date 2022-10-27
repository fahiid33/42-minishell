/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 03:43:13 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/26 00:17:54 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	if ((unsigned int)ft_strlen(s) < start || len == 0)
		return (ft_strdup(""));
	str = (char *)f_malloc(sizeof(char) * len + 1);
	if (str == NULL)
		return (NULL);
	while (s[i])
	{
		if (j < len && i >= start)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	return (str);
}

void	switch_var(int *i)
{
	if (*i == 0)
		*i = 1;
	else
		*i = 0;
}

void	*f_malloc(size_t size)
{
	void	*tmp;

	tmp = malloc(size);
	if (!tmp)
		return (NULL);
	g_vars.alloc[g_vars.index] = tmp;
	g_vars.index++;
	return (tmp);
}

void	*realloc_array(char **arg, char *str)
{
	char	**new_arg;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (arg && arg[i])
		i++;
	new_arg = (char **)f_malloc(sizeof(char *) * (i + 2));
	while (arg && j < i)
	{
		new_arg[j] = ft_strdup(arg[j]);
		j++;
	}
	new_arg[j++] = ft_strdup(str);
	new_arg[j] = NULL;
	return (new_arg);
}

char	*ft_strdup(char *str)
{
	char	*dup;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	dup = f_malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

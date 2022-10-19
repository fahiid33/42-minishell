/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 04:33:18 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/18 04:44:17 by amoubare         ###   ########.fr       */
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

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest && dest[i] != '\0')
		i++;
	j = 0;
	while (src && src[j] != '\0' )
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*join_3_str(char *s1, char *s2, char *s3)
{
	char	*str;
	int		len;
	int		len1;

	if (s3)
		len1 = ft_strlen(s3);
	else
		len1 = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + len1;
	str = (char *)f_malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str = ft_strcpy(str, s1);
	str = ft_strcat(str, s2);
	if (s3)
	{
		str = ft_strcat(str, s3);
		return (str);
	}
	return (str);
}

static int	countword(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if ((i == 0 || s[i - 1] == c) && s[i] != c)
			count++;
		i++;
	}
	return (count);
}

char	*allocwords(char const *s, char c, int i)
{
	int		j;
	char	*str;

	j = i;
	while (s[j] != c && s[j])
	{
		j++;
	}
	str = f_malloc(sizeof(char) * j + 1);
	if (str == NULL)
		return (0);
	j = 0;
	while (s[i] && s[i] != c)
	{
		str[j] = s[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		j;

	if (!s)
		return (0);
	str = f_malloc(sizeof(char *) * (countword(s, c) + 1));
	if (str == NULL)
		return (0);
	i = 0;
	j = 0;
	while (s[i])
	{
		if ((i == 0 || s[i - 1] == c) && s[i] != c)
		{
			str[j] = allocwords(s, c, i);
			j++;
		}
		i++;
	}
	str[j] = NULL;
	return (str);
}

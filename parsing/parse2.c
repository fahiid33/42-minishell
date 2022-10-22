/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 03:22:18 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/22 05:21:06 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    fill_sequences_adv(int *sequences, int *o, int len, int x)
{
    while (len--)
    {
        sequences[*o] = x;
        (*o)++;
    }
}

int expand_digit(char *value, char **result, int *i)
{
	if (value[*i] == '0')
	{
		*result = ft_strjoin(*result, my_getenv(g_vars.my_env, "0"));
		(*i)++;
		return(1) ;
	}
	(*i)++;
	while (value[*i] && !is_token(value[*i]) && value[*i] != '$')
	{
		*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
		(*i)++;
	}
	(*i)--;
	return (0);
}

int    simple_expand(char *value, char **result, int *i)
{
    char 	*to_expand;
	int		len;

    to_expand = ft_strdup("");
	len = 0;
    while (is_alphanum(value[*i]))
    {
        to_expand = ft_strjoin(to_expand, ft_strndup(&value[*i], 1));
        (*i)++;
    }
    if (my_getenv(g_vars.my_env, to_expand))
	{
        *result = ft_strjoin(*result, my_getenv(g_vars.my_env, to_expand));
		len = ft_strlen(my_getenv(g_vars.my_env, to_expand));
	}
    else
        *result = ft_strjoin(*result, ft_strdup(""));
    (*i)--;
	return (len);
}

int    expand_in_dq(char *value, char **result, int *i, int f)
{
    char    *dq;

    dq = ft_strdup("");
	if((value[*i] == '\0' || ft_int_strchr(&value[*i], 34 ) == -1) && !f)
	{
		errors(2);
		return (-1);
	}
    else if (value[*i] && value[*i] == 34)
    {
        (*i)++;
        return (0);
    }
    while (value[*i] && value[*i] != 34)
    {
        dq = ft_strjoin(dq, ft_strndup(&value[*i], 1));
        (*i)++;
    }
    *result = ft_strjoin(*result, dq_content(dq));
    return(ft_strlen(dq_content(dq)));
}

int	collect_squote(char *value, char **result, int *i, int f)
{
	int len;

	len = 0;
	*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
	(*i)++;
	if(ft_int_strchr(&value[*i], 39) == -1 && !f)
	{
		errors(2);
		return(-1);
	}
	len++;
	while (value[*i] && value[*i] != 39)
	{
		*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
		(*i)++;
		len++;
	}
	if (value[*i])
	{
		*result = ft_strjoin(*result, ft_strndup(&value[*i], 1));
		len++;
	}
	return (len);
}

int	collect_char(char *value, char **result, int i)
{
	*result = ft_strjoin(*result, ft_strndup(&value[i], 1));
	return(1);
}

int	expand_dd(char **result)
{
	int len;

	len = ft_strlen(ft_itoa(g_vars.pid));
	*result = ft_strjoin(*result, ft_itoa(g_vars.pid));
	return (len);
}

int	collect_dollar(char **result)
{
	*result = ft_strjoin(*result, ft_strdup("$"));
	return(1);
}

int	get_exit_status(char **result)
{
	int len;

	len = ft_strlen(ft_itoa(g_vars.exit_status));
	*result = ft_strjoin(*result, ft_itoa(g_vars.exit_status));
	return (len);
}


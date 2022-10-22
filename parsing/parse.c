/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:17:49 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/22 04:53:28 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*parsing(t_token *tokens)
{
	int		*sequences;
	t_token	*tmp;
	char	*exd;

	tmp = tokens->next;
	tokens = tokens->next;
	sequences = f_malloc(sizeof(int) * 10000);
	exd = ft_strdup("");
	while (tokens->e_type != END)
	{
		if (tokens->e_type == WORD)
		{
			if (there_is_dollar(tokens->value))
			{
				tokens->value = expand_dollar(tokens->value, sequences, 0);
				if (tokens->value == NULL)
					return (NULL);
			}
			else
			{
				fill_sequences(ft_strlen(tokens->value), sequences);
			}
			tokens->value = remove_quotes(ft_strdup(tokens->value), sequences);
			if (tokens->value == NULL)
				return (NULL);
		}
		if ((tokens->e_type == GREATANDGREAT || tokens->e_type == GREAT
				|| tokens->e_type == LESS)
			&& tokens->next && tokens->next->value[0] == '$')
		{
			tokens = tokens->next;
			tokens->value = ft_strdup("*");
		}
		else if (tokens->e_type == LESSANDLESS && tokens->next->e_type == WORD)
		{
			tokens = tokens->next;
			fill_sequences(ft_strlen(tokens->value), sequences);
			tokens->value = remove_quotes(tokens->value, sequences);
			if (tokens->value == NULL)
				return (NULL);
		}
		tokens = tokens->next;
	}
	free(sequences);
	return (tmp);
}

int	there_is_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}
void	print_int_tab(int *tab)
{
	int	i;

	i = 0;
	while (tab[i] == 2 || tab[i] == 1)
	{
		printf("%d", tab[i]);
		i++;
	}
	printf("\n");
}
char	*expand_dollar(char *value, int *sequences, int f)
{
	int		i;
	int		o;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	o = 0;
	result = ft_strdup("");
	while (value[i] != '\0')
	{
		if (value[i] == '$')
		{
			i++;
			if (value[i] == '\0')
			{
				fill_sequences_adv(sequences, &o, collect_dollar(&result), 1);
				break;
			}
			else if (is_digit(value[i]))
			{
				if (expand_digit(&value[i], &result, &i))
					continue ;
			}
			else if (value[i] == '$')
				fill_sequences_adv(sequences, &o, expand_dd(&result), 1);
			else if (value[i] == '?')
				fill_sequences_adv(sequences, &o, get_exit_status(&result), 1);
			else if ((value[i] == 39 || value[i] == 34))
				continue ;
			else if ((value[i] && !is_alpha(value[i]) && !is_digit(value[i])))
			{
				fill_sequences_adv(sequences, &o, collect_dollar(&result), 1);
				continue ;
			}
			else
				simple_expand(value, &result, &i);
		}
		else if (value[i] == 34)
		{
			i++;
			int ok = expand_in_dq(value, &result, &i, f);
			if(ok == -1)
				return(NULL);
			else if(ok == 0)
				continue ;
			else
				fill_sequences_adv(sequences, &o, ok, 1);
		}
		else if (value[i] && value[i] == 39)
		{
			if(collect_squote(value, &result, &i, f) == -1)
				return(NULL);
			fill_sequences_adv(sequences, &o, collect_squote(value, &result, &i, f), 2);
		}
		else
			fill_sequences_adv(sequences, &o, collect_char(value, &result, i), 1);
		if (!value[i])
			break ;
		i++;
	}
	return (result);
}

char	*remove_quotes(char *value, int *sequences)
{
	int		i;
	char	q;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (value[i] != '\0')
	{
		if (sequences[i] != 1)
		{
			if (value[i] == 34 || value[i] == 39)
			{
				q = value[i];
				i++;
				if (q == 39 || q == 34)
				{
					if (ft_int_strchr(&value[i], q) == -1 && g_vars.g_err != 1)
					{
						errors(2);
						return (NULL);
					}
				}
				if (g_vars.g_err != 1)
				{
					while (value[i] && value[i] != q)
					{
						result = ft_strjoin(result, ft_strndup(&value[i], 1));
						i++;
					}
				}
				i++;
			}
			if (g_vars.g_err != 1 && value[i] != '\0' && value[i] != 34
				&& value[i] != 39)
			{
				result = ft_strjoin(result, ft_strndup(&value[i], 1));
				i++;
			}
		}
		else
		{
			if (g_vars.g_err != 1)
				result = ft_strjoin(result, ft_strndup(&value[i], 1));
			i++;
		}
	}
	return (result);
}

void	fill_sequences(int len, int *sequences)
{
	int	i;

	i = 0;
	while (i < len)
	{
		sequences[i] = 2;
		i++;
	}
}

void	fill_tparse(t_token *tokens, t_parse **parse)
{
	int		type;
	t_parse	*tmp;

	tmp = *parse;
	while (tokens)
	{
		if (tokens->e_type == WORD)
		{
			if (!tmp->cmd)
				tmp->cmd = ft_strdup(tokens->value);
			else
				tmp->argv = realloc_array(tmp->argv, tokens->value);
			tokens = tokens->next;
		}
		else if (tokens->e_type == GREAT || tokens->e_type == LESS
			|| tokens->e_type == LESSANDLESS || tokens->e_type == GREATANDGREAT)
		{
			if (tokens->next->e_type != WORD)
			{
				errors(258);
				return ;
			}
			type = tokens->e_type;
			tokens = tokens->next;
			if (!tmp->redir)
				tmp->redir = init_redir(tokens->value, type);
			else
				tmp->redir = add_redir(tmp->redir, tokens->value, type);
			tokens = tokens->next;
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

char	*dq_content(char *value)
{
	int		i;
	char	*result;
	char	*expd;

	i = 0;
	result = ft_strdup("");
	while (value[i])
	{
		expd = ft_strdup("");
		if (value[i] == '$' && value[i + 1]
			&& (is_alphanum(value[i + 1]) || value[i + 1] == '?'
				|| value[i + 1] == '$'))
		{
			i++;
			if (is_digit(value[i]))
			{
				if (value[i] == '0')
				{
					result = ft_strjoin(result, my_getenv(g_vars.my_env, "0"));
				}
				i++;
				while (value[i] && !is_token(value[i]) && value[i] != '$')
				{
					result = ft_strjoin(result, ft_strndup(&value[i], 1));
					i++;
				}
				i--;
			}
			else if (value[i] == '$')
			{
				result = ft_strjoin(result, ft_itoa(g_vars.pid));
			}
			else if (value[i] == '?')
				result = ft_strjoin(result, ft_itoa(g_vars.exit_status));
			else if ((value[i] == 39 || value[i] == 34))
			{
				result = ft_strjoin(result, ft_strndup(&value[i], 1));
				i++;
				continue ;
			}
			else if ((value[i] && !is_alpha(value[i]) && !is_digit(value[i])))
			{
				result = ft_strjoin(result, "$");
				continue ;
			}
			else
			{
				while ((is_alpha(value[i]) || is_digit(value[i])))
				{
					expd = ft_strjoin(expd, ft_strndup(&value[i], 1));
					i++;
				}
				if (my_getenv(g_vars.my_env, expd))
					result = ft_strjoin(result, my_getenv(g_vars.my_env, expd));
				else
					result = ft_strjoin(result, ft_strdup(""));
				i--;
			}
		}
		else
			result = ft_strjoin(result, ft_strndup(&value[i], 1));
		i++;
	}
	return (result);
}

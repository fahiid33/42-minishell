/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:17:49 by amoubare          #+#    #+#             */
/*   Updated: 2022/10/19 23:08:03 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


t_token	*parsing(t_token *tokens)
{
	int *sequences;
	t_token	*tmp = tokens->next;
	tokens = tokens->next;
	sequences = f_malloc(sizeof(int) * 10000);
	while (tokens->e_type != END)
	{
		if(tokens->e_type == WORD)
		{
			if(there_is_dollar(tokens->value))
			{
				tokens->value = expand_dollar(tokens->value, sequences);
				if(tokens->value == NULL)
					return(NULL);
			}
			else
			{
				fill_sequences(ft_strlen(tokens->value), sequences);
			}
			tokens->value = remove_quotes(ft_strdup(tokens->value), sequences);
		}
		else if(tokens->e_type == LESSANDLESS)
		{
			tokens = tokens->next;
			fill_sequences(ft_strlen(tokens->value), sequences);
			tokens->value = remove_quotes(tokens->value, sequences);
		}
		tokens = tokens->next;
	}
	free(sequences);
	return(tmp);
}

int		there_is_dollar(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_dollar(char *value, int *sequences)
{
	int i = 0;
	int o = 0;
	int j = 0;
	char *expnd = ft_strdup("");
	char *result = ft_strdup("");
	char *dq = ft_strdup("");
	while(value[i] != '\0')
	{
		if(value[i] == '$')
		{
			i++;
			if(is_digit(value[i]))
			{
				if(value[i] == '0')
				{
					result = ft_strjoin(result, my_getenv(g_vars.my_env, "0"));
					i++;
					continue ;
				}
				i++;
				while(value[i] && !is_token(value[i]))
				{
					result = ft_strjoin(result, ft_strndup(&value[i], 1));
					i++;
				}
			}
			else if(value[i] == '\0')
			{
				result = ft_strjoin(result, "$");
				sequences[o++] = 1;
				break;
			}
			else if(value[i] == '$')
			{
				result = ft_strjoin(result, "$$");
				sequences[o++] = 1;
				sequences[o++] = 1;
			}
			else if(value[i] == '?')
				result = ft_strjoin(result, ft_itoa(g_vars.exit_status));
			else if((value[i] == 39 || value[i] == 34))
			{
				continue ;
			}
			else if((value[i] && !is_alpha(value[i]) && !is_digit(value[i])))
			{
				result = ft_strjoin(result, "$");
				continue ;
			}
			else
			{
				while((is_alpha(value[i]) || is_digit(value[i])))
				{
					expnd = ft_strjoin(expnd, ft_strndup(&value[i], 1));
					i++;
				}
				if(my_getenv(g_vars.my_env, expnd))
					result = ft_strjoin(result, my_getenv(g_vars.my_env, expnd));
				else
					result = ft_strjoin(result, ft_strdup(""));
				i--;
			}
		}
		else if(value[i] == 34)
		{
			dq = ft_strdup("");
			i++;
			if(value[i] == '\0')
			{
				errors(2);
				return(NULL);
			}
			else if(value[i] && value[i] == 34)
			{
				i++;
				continue ;
			}
			while(value[i] && value[i] != 34)
			{
				dq = ft_strjoin(dq, ft_strndup(&value[i], 1));
				i++;
			}
			j = 0;
			while(j < ft_strlen(dq_content(dq)))
			{
				sequences[o] = 1;
				o++;
				j++;
			}
			j = 0;
			result = ft_strjoin(result, dq_content(dq));
		}
		else if(value[i] && value[i] == 39)
		{
			result = ft_strjoin(result, ft_strndup(&value[i], 1));
			i++;
			if(ft_int_strchr(&value[i], 39) == -1)
			{
				errors(2);
				return(NULL);
			}
			sequences[o] = 2;
			o++;
			while(value[i] && value[i] != 39)
			{
				result = ft_strjoin(result, ft_strndup(&value[i], 1));
				i++;
				sequences[o] = 2;
				o++;
			}
			if (value[i])
			{
				result = ft_strjoin(result, ft_strndup(&value[i], 1));
			}
			sequences[o] = 2;
			o++;
		}
		else
		{
			result = ft_strjoin(result, ft_strndup(&value[i], 1));
			sequences[o] = 1;
			o++;
		}
		i++;
	}
	return(result);
}
char	*remove_quotes(char *value, int *sequences)
{
	int i;
	i = 0;
	char q;
	char *result = ft_strdup("");
	while(value[i] != '\0')
	{
		if(sequences[i] != 1)
		{
			if(value[i] == 34 || value[i] == 39)
			{
				q = value[i];
				i++;
				if(q == 39 || q == 34)
				{
					if(ft_int_strchr(&value[i], q) == -1 && g_vars.g_err != 1)
					{
						errors(2);
						break ;
					}
				}
				if (g_vars.g_err != 1)
				{
					while(value[i] && value[i] != q)
					{
						result = ft_strjoin(result, ft_strndup(&value[i], 1));
						i++;
					}
				}
				i++;
			}
			if (g_vars.g_err != 1 && value[i] != '\0' && value[i] != 34 && value[i] != 39)
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
	int i = 0;
	while(i < len)
	{
		sequences[i] = 2;
		i++;
	}
}


void	fill_tparse(t_token *tokens, t_parse **parse)
{
	int type;
	t_parse *tmp;

	tmp = *parse;
	while(tokens)
	{
		if(tokens->e_type == WORD)
		{
			if(!tmp->cmd)
				tmp->cmd = ft_strdup(tokens->value);
			else
				tmp->argv = realloc_array(tmp->argv, tokens->value);
			tokens = tokens->next;
		}
		else if (tokens->e_type == GREAT || tokens->e_type == LESS
			|| tokens->e_type == LESSANDLESS || tokens->e_type == GREATANDGREAT)
		{
				if(tokens->next && tokens->next->e_type != WORD)
					errors(258);
				type = tokens->e_type;
				tokens = tokens->next;
				if(!tmp->redir)
					tmp->redir = init_redir(tokens->value, type);
				else
					tmp->redir = add_redir(tmp->redir, tokens->value, type);
				tokens = tokens->next;
		}
		if(tokens->e_type == PIPE || tokens->e_type == END)
		{
			if(tokens->next && tokens->e_type == PIPE && tokens->next->e_type == END)
			{
				errors(3);
				return;
			}
			tmp = add_command(tmp);
			tmp = tmp->next;
			tokens = tokens->next;
		}
	}
}

char *dq_content(char *value)
{
	int i;
	char *result = ft_strdup("");
	char *expnd = ft_strdup("");
	i = 0;
	while(value[i])
	{
		expnd = ft_strdup("");
		if(value[i] == '$' && value[i + 1] && (is_alphanum(value[i + 1]) || value[i + 1] == '?'))
		{
			i++;
			if(is_digit(value[i]))
			{
				i++;
				while(value[i] && !is_token(value[i]))
				{
					result = ft_strjoin(result, ft_strndup(&value[i], 1));
					i++;
				}
			}
			else if(value[i] == '$')
			{
				result = ft_strjoin(result, "$$");
			}
			else if(value[i] == '?')
				result = ft_strjoin(result, ft_itoa(g_vars.exit_status));
			else if((value[i] == 39 || value[i] == 34))
			{
				result = ft_strjoin(result, ft_strndup(&value[i], 1));
				i++;
				continue ;
			}
			else if((value[i] && !is_alpha(value[i]) && !is_digit(value[i])))
			{
				result = ft_strjoin(result, "$");
				continue ;
			}
			else
			{
				while ((is_alpha(value[i]) || is_digit(value[i])))
				{
					expnd = ft_strjoin(expnd, ft_strndup(&value[i], 1));
					i++;
				}
				if (my_getenv(g_vars.my_env, expnd))
					result = ft_strjoin(result, my_getenv(g_vars.my_env, expnd));
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
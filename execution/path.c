/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 23:36:59 by fstitou           #+#    #+#             */
/*   Updated: 2022/10/30 20:45:33 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_cmd(t_parse *cmd)
{
	int		u;
	char	*save;

	u = 0;
	save = ft_strdup(cmd->cmd);
	if (!cmd->cmd)
		return ;
	if (is_upper_case(cmd->cmd))
	{
		u = 1;
		cmd->cmd = ft_lowercase(cmd->cmd);
	}
	if (builtins_cases(cmd) && ft_strcmp(cmd->cmd, "echo") != 0 && u)
	{
		cmd->cmd = save;
		return ;
	}
	if (!ft_strcmp(cmd->cmd, "echo") && u)
	{
		cmd->cmd = save;
		return ;
	}
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*str_join(char *s1, char *s2)
{
	char	*copy;
	int		i;
	int		j;

	copy = f_malloc(sizeof(char) * (ft_int_strchr(s1, 0)
				+ft_int_strchr(s2, 0) + 2));
	i = 0;
	j = 0;
	while (s1[j])
		copy[i++] = s1[j++];
	copy[i++] = '/';
	j = 0;
	while (s2[j])
		copy[i++] = s2[j++];
	copy[i] = '\0';
	return (copy);
}

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (cmd && cmd[0] != '\0' && path && ft_int_strchr(path, ':') > -1)
	{
		dir = ft_strndup(path, ft_int_strchr(path, ':'));
		bin = str_join(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += ft_int_strchr(path, ':') + 1;
	}
	return (cmd);
}

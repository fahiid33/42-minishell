/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 18:20:30 by fahd              #+#    #+#             */
/*   Updated: 2022/11/01 22:12:37 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <ctype.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <stdio.h>
# include <stddef.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_lex
{
	char	*cmd;
	int		i;
	char	c;
}	t_lex;

typedef struct s_token
{
	char			*value;
	struct s_token	*next;
	enum
	{
		WORD,
		PIPE,
		GREATANDGREAT,
		LESSANDLESS,
		GREAT,
		LESS,
		END,
	} e_type;
}		t_token;

typedef struct s_vars
{
	char	*result;
	int		*seq;
	int		o;
	int		check;
	int		ss;
}	t_vars;

typedef struct s_redir
{
	char			*file;
	int				e_type;
	int				fdout;
	int				fdin;
	struct s_redir	*next;

}		t_redir;

typedef struct s_parse
{
	char			*cmd;
	char			**argv;
	t_redir			*redir;
	struct s_parse	*next;
}		t_parse;

typedef struct s_env
{
	char			*key;
	char			*val;
	char			sep;
	struct s_env	*next;
}		t_env;

typedef struct s_minishell
{
	char	*line;
	int		exit_status;
	int		exit_sig;
	int		flag;
	int		g_err;
	pid_t	pid;
	void	*alloc[1000000];
	int		index;
	t_env	*my_env;
}		t_minishell;

t_minishell	g_vars;

// parse
int		parsing_helper(t_token **tokens, t_token *prev, t_vars *p);
int		expand_word_norm(char *value, int *i, t_vars **p, int f);
int		collect_dq_check(char *value, int *i, t_vars **p, int f);
int		collect_sq_check(char *value, int *i, t_vars **p, int f);
int		expand_dollar(char *value, int *i, t_vars **p);
void	errors2(int exit_status);
char	*_2d_arr_to_str(char **arr);
char	*to_exp(char *to_expand, int flag);
char	*split_args(char *str, int flag);
int		there_is_space(char *str);
t_vars	*init_vars(void);
int		fill_cmd(t_token **tokens, t_parse **tmp);
int		expand_digit_helper(t_vars **p, char *value, int *i);
int		expand_dq(char *value, char **result, int *i);
int		fill_redir_file(t_token **tokens, t_parse **tmp);
void	fill_cmd_args(t_token **tokens, t_parse **tmp);
void	ambiguous_redirect(t_token **tokens);
void	collect_expanded(char *value, char **result, int *i);
int		collect_inside_quotes(char *value, int *i, char **result);
int		parse_word(t_token **tokens, t_vars *p);
int		parse_delimiter(t_token **tokens, t_vars *p);
int		get_exit_status(char **result);
int		collect_char(char *value, char **result, int i);
int		expand_dd(char **result);
int		collect_dollar(char **result);
int		collect_squote(char *value, char **result, int *i, int f);
int		expand_in_dq(char *value, char **result, int *i, int f);
t_vars	*fill_sequences_adv(t_vars *p, int len, int x);
int		only_space(char *str, char c);
int		simple_expand(char *value, char **result, int *i, int flag);
int		expand_digit(char *value, char **result, int *i);
void	collect_to_token(int *i, int *j, t_lex **lex, char **val);
void	continue_to_dquote(int *i, t_lex **lex, char **val);
void	continue_to_squote(int *j, t_lex **lex, char **val);
int		collect_to_token_repeat(int *i, int *j, t_lex **lex, char **val);
void	print_int_tab(int *tab);
void	switch_var(int *i);
int		ft_is_space(void);
char	*dq_content(char *value);
void	fill_tparse(t_token *tokens, t_parse **parse);
t_parse	*init_cmd(void);
t_parse	*add_command(t_parse *commad);
t_parse	*add_command_back(t_parse *lst, t_parse *new);
t_redir	*init_redir(char *val, int type);
t_redir	*lst_add_back_redir(t_redir *lst, t_redir *new);
t_redir	*add_redir(t_redir *redir, char *val, int type);
void	advance_lex(t_lex *lex);
t_token	*parsing(t_token *tokens);
void	fill_sequences(int len, t_vars	*p);
int		there_is_dollar(char *str);
char	*expand_word(char *value, t_vars *p, int f);
char	*remove_quotes(char *value, int *sequences);
t_token	*lst_add_back(t_token *lst, t_token *new);
t_token	*init_create_tokens(t_token *tokens, char *line);
t_token	*init_token(char *val, int type);
t_token	*create_tokens(t_lex *lex, t_token *tokens);
void	token_pipe(t_lex *lex, t_token *tokens);
void	token_great(t_lex *lex, t_token *tokens);
void	token_less(t_lex *lex, t_token *tokens);
void	token_word(t_lex *lex, t_token *tokens);
void	end_token(t_token *tokens);
void	*f_malloc(size_t size);
void	save_address(char *add);
void	_errors(int err, char *str);
int		read_error(char *file, int exec);
void	*realloc_array(char **arg, char *str);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
int		is_digit(char c);
int		is_token(char c);
char	*ft_strndup(char *str, int i);
int		counter(int n);
char	*minmax(int n, char *str);
char	*negative(int *n, char *str, int count);
char	*ft_itoa(int n);
int		is_alpha(char c);
int		is_alphanum(char c);
int		ft_strlen(char *str);
int		ft_int_strchr(const char *s, int c);
void	errors(int exit_status);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
int		only_enter(void);
int		ft_strcmp(char *s1, char *s2);
char	*allocwords(char const *s, char c, int i);
char	**ft_split(char const *s, char c);

// execution

void	free_unused(int frr, char *s1, char *s2);
int		is_upper_case(char *str);
char	*ft_upper_case(char *str);
void	dup_files(int fin, int fout);
void	file_error(char *filename, int am);
char	*ft_strjoino(char *s1, char *s2, int frr);
void	join_string2(char *str, char *s2, int j);
t_env	*lst_new(char *key, char sep, char *val);
int		open_files(t_redir *redir, int mode);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_itoa(int n);
void	check_cmd(t_parse *cmd);
char	*ft_lowercase(char *str);
int		check_minus(char *arg, char c);
int		str_digit(char *str);
int		ambg_redir(char *file);
int		str_is_alnum(char *str);
void	*f_malloc(size_t size);
char	*join_3_str(char *s1, char *s2, char *s3);
int		ft_int_strchr(const char *s, int c);
int		exec_builtins(t_parse *commands);
void	exec_pipeline(t_parse *commands, t_env **env);
char	**ft_split(char const *s, char c);
int		pwd(void);
void	init_env(char **env);
char	*ft_strcpy(char *dest, char *src);
char	*my_getenv(t_env *env, char *key);
char	*my_getenv_key(t_env **env, char *key);
void	update_export(t_env **env, char *key, char sep, char *val);
char	*get_path(char *cmd, char **env);
void	execute(t_parse *command, t_env **env);
int		check_exp_arg(char *to_check);
int		builtins_cases(t_parse *cmd);
int		cd(t_parse *head);
void	lst_add_backenv(t_env **lst, t_env *new);
int		export(t_parse *head);
void	check_numb(char *str);
int		my_exit(t_parse *cmd);
int		echo(t_parse *cmd);
int		unset(t_parse *cmd);
void	wrong_cmd(char *cmd);
void	wrong_cmd_helper(char *error, int w);
void	c_signal(void);
void	open_redir(t_parse *head, int exec);
void	pipe_redir(t_parse *cmd, int in, int index, int *fd);
int		simple_cmd(t_parse *cmd);
void	read_heredocs(t_parse *command);
int		only_enter(void);
int		env(void);
int		is_piped(void);
char	*join_3_str(char *s1, char *s2, char *s3);
char	*ft_strcat(char *dest, char *src);

#endif

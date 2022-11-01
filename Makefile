
# CPPFLAGS = -I/Users/fstitou/.brew/opt/readline/inlclude
# LDFLAGS = -L/Users/fstitou/.brew/opt/readline/lib
CPPFLAGS = -I/goinfre/fstitou/homebrew/opt/readline/include
LDFLAGS = -L/goinfre/fstitou/homebrew/opt/readline/lib

NAME = minishell
CC = cc

CFLAGS = -Wall -Wextra -Werror
SANITIZE = -g -fsanitize=address
		
SRC = minishell.c execution/builtins.c execution/cd.c execution/echo.c execution/env.c execution/exec_errors.c \
execution/execute.c execution/execute_utils.c execution/exit.c execution/export-env-utils.c \
execution/export-env.c execution/export.c execution/files.c execution/here_doc.c execution/path.c \
execution/pwd.c execution/redir_helper.c execution/redirections.c execution/signals.c execution/unset.c \
parsing/exec_utils.c parsing/exec_utils2.c parsing/expand_word.c parsing/fill_parse.c \
parsing/init.c parsing/itoa.c parsing/parse.c parsing/parse_utils.c parsing/parse_utils2.c \
parsing/parse_utils3.c parsing/token_utils.c parsing/token_word.c parsing/tokenize.c \
parsing/utils.c parsing/utils15.c parsing/utils2.c parsing/utils3.c parsing/utils4.c


OBJ = $(SRC:.c=.o)


all : $(NAME)

$(NAME):
	@$(CC) $(CPPFLAGS) $(SRC) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) -lreadline  -o $(NAME) $(SANITIZE)
	@echo "👍👍👍"

clean :
	@rm -f $(OBJ)
	@echo "🚮🚮🚮"

fclean : clean
	@rm -f $(NAME)
	@echo "🗑️ 🗑️ 🗑️"

re : fclean all
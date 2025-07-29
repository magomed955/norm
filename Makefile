NAME = minishell

SRC = src/minishell.c envi/get_env.c envi/utils_env.c builtin/cd.c builtin/cd_utils.c builtin/env.c builtin/echo.c builtin/export.c \
	builtin/export_utils.c builtin/export_utils2.c builtin/exit.c builtin/exit_utils.c builtin/pwd.c builtin/unset.c builtin/is_builtin.c \
	builtin/free_array.c parsing/token.c parsing/token2.c parsing/handle_prompt.c parsing/handle_prompt2.c parsing/handle_cmd.c parsing/expand.c \
	parsing/handle_cmd2.c parsing/handle_cmd_parsing.c parsing/handle_cmd_redirections.c parsing/handle_cmd_init.c exec/exec.c exec/free.c \
	exec/free_utils.c exec/heredoc.c exec/heredoc_utils.c exec/heredoc_utils2.c exec/exec_utils.c exec/exec_cleanup.c exec/exec_builtin.c \
	exec/exec_redirect.c exec/exec_path.c exec/exec_fork.c exec/exec_signals.c exec/exec_fds.c signals/signals.c signals/signals_utils.c \
	syntaxe/error_messages.c

OBJS = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror -g

CC = gcc

LIBS = -lreadline
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(FLAGS) $(OBJS) $(LIBS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	make clean -C libft

fclean: clean
	rm -rf $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
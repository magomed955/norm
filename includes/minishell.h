/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:29:38 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 22:54:11 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define QUOTE 1
# define DQUOTE 2
# define PIPE 3
# define REDIN 4
# define REDOUT 5
# define WORD 6
# define APPEND 7
# define HEREDOC 8
# define BUFFER_SIZE 4096
# define READWRITE 9
# define OR_IF 10
# define AND_IF 11
# define SLASH_SEQ 12

extern int				g_exit_status;

typedef struct s_exit
{
	int					*exit;
}						t_exit;

typedef struct s_env
{
	char				**env;
	char				**export_only;
}						t_env;

typedef struct s_token
{
	char				*content;
	int					type;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_redir_in
{
	int					type;
	char				*value;
	struct s_redir_in	*next;
}						t_redir_in;

typedef struct s_exec_context
{
	int					*fds;
	int					index;
	int					is_last;
	int					total_cmds;
}						t_exec_context;

typedef struct s_cmd
{
	char				*cmd;
	char				**args;
	char				*infile;
	char				*outfile;
	int					append;
	t_redir_in			*redir_in;
	char				**heredoc;
	int					nb_heredoc;
	int					heredoc_fd;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_data
{
	t_exit				*exit;
	t_env				*env;
	t_token				*token;
	t_cmd				*cmd;
	int					should_exit;
	int					*fds;
	int					cmd_count;
}						t_data;

typedef struct s_pipeline_ctx
{
	t_data				*data;
	t_token				*tokens;
	int					*fds;
	int					count;
}						t_pipeline_ctx;

typedef struct s_wait_ctx
{
	int					status;
	pid_t				finished_pid;
	pid_t				last_pid;
	t_data				*data;
}						t_wait_ctx;

typedef struct s_expand_ctx
{
	char				*prompt;
	t_data				*data;
	char				*buffer;
	int					*i;
	int					*j;
}						t_expand_ctx;

typedef struct s_token_ctx
{
	char				*prompt;
	t_token				**head;
	char				*buffer;
	int					*i;
	int					*j;
	int					quote_state;
}						t_token_ctx;

typedef struct s_tokenize_vars
{
	t_token				**head;
	char				*buffer;
	int					i;
	int					j;
	int					quote_state;
}						t_tokenize_vars;

void					copy_env(t_data *data, char **envp);
void					execute_cmd(char *path, char **argv, t_data *data);
char					*prepare_path(char *cmd, t_data *data);
char					*get_path(char *cmd, t_data *data, char **argv);
void					update_shlvl(t_data *data);
void					free_env(t_env *env);
void					free_tab(char **str);
void					free_cmd(t_cmd *cmd);
void					free_tokens(t_token *token);
void					free_data(t_data *data);
void					fill_heredocs_from_tokens(t_token *token, t_cmd *cmd);
void					allocate_heredoc(t_token *token, t_cmd *cmd);
void					process_heredocs(t_cmd *cmd, t_data *data);
int						handle_no_command(t_cmd *cmds, t_data *data,
							t_token *tokens, char *prompt);
int						error_handling(int err, t_data *data);
void					handle_sigint_status(t_data *data);
void					ft_cd(char **args, t_data *data);
void					enable_echoctl(void);
void					disable_echoctl(void);
void					ft_env(t_data *data);
void					ft_pwd(t_data *data);
int						is_builtin(char *prompt);
int						is_n_option(const char *str);
void					ft_echo(t_data *data, char **args);
void					ft_unset(char **args, t_data *data);
void					ft_exit(char **args, t_data *data);
int						ft_export(t_data *data, char **args);
void					export_display(t_data *data);
void					ft_sort_ascii(char **array);
int						is_parent_builtin(char *prompt);
char					**ft_add_to_array(char **array, const char *new_str);
void					exec_builtin(char **argv, t_data *data);
void					free_array(char **array);
void					add_token(t_token **head, char *content, int type);
char					**ft_strdup_array(char **array);
int						is_valid_export_identifier(const char *s);
void					handle_dquote(char *prompt, t_token **head, int *j);
void					ft_puterror(const char *prefix, const char *arg,
							const char *msg);
void					handle_quote(char *prompt, t_token **head, int *j);
void					export_add_or_update(t_data *data, char *arg);
char					*get_env_value(char *name, t_data *data);
char					*get_dollar_value(char *prompt, int j);
t_data					*init_data(char **envp);
void					handle_pipe(t_token **head, char buffer[BUFFER_SIZE],
							int *i, int *j);
void					handle_space(t_token **head, char buffer[BUFFER_SIZE],
							int *i, int *j);
void					handle_redin(t_token **head, char buffer[BUFFER_SIZE],
							int *i, int *j);
void					handle_redout(t_token **head, char buffer[BUFFER_SIZE],
							int *i, int *j);
void					handle_append(t_token **head, char buffer[BUFFER_SIZE],
							int *i, int *j);
void					handle_heredoc(t_token **head, char buffer[BUFFER_SIZE],
							int *i, int *j);
void					update_quote_state(char c, int *quote_state);
int						create_heredoc_pipe(t_cmd *cmd, t_data *data);
t_cmd					*init_struct(void);
t_cmd					*parse_tokens(t_token *token);
t_token					*tokenize(char *prompt, t_data *data);
char					**build_argv(t_cmd *cmd);
void					print_tokens(t_token *head);
void					print_cmds(t_cmd *cmd);
int						*malloc_fds(int n);
int						create_fds(t_cmd *cmd, int **fds_out);
int						redirect_input(t_cmd *cmd, int *fds, int index);
int						redirect_output(t_cmd *cmd, int *fds, int index,
							int is_last);
void					execute_one_cmd(t_cmd *cmd, t_exec_context *ctx,
							t_data *data, t_token *tokens);
void					execute_all_cmd(t_cmd *cmd, t_data *data,
							t_token *tokens);
void					handle_sigint(int sig);
void					free_single_cmd(t_cmd *cmd);
void					handle_sigint_heredoc2(int sig);
void					handle_sigint_heredoc(void);
void					cleanup_and_exit(char **argv, t_data *data,
							int exit_code);
void					close_all_fds(int *fds, int count);
void					close_fds(void);
void					add_redir_in(t_cmd *cmd, int type, const char *value);
int						is_operator(int type);
int						is_redirection(int type);
int						has_valid_command(t_token *tokens);
int						print_syntax_error(t_token *token);
int						validate_syntax(t_token *tokens);
char					*ft_getenv(char **envp, const char *key);
int						count_env_vars(char **envp);
void					cleanup_env_allocation(char **env, int up_to);
int						allocate_env_copy(t_data *data, char **envp, int count);
char					*find_path_env(t_data *data);
char					*build_full_path(char *dir, char *cmd);
int						set_env_var(t_data *data, const char *key,
							const char *value);
int						update_pwd(t_data *data, char *oldpwd);
int						get_current_dir(char *cwd);
char					*backup_current_dir(void);
int						change_directory(char *path);
int						validate_cd_args(char **args, t_data *data);
int						is_numeric(const char *str);
int						adder(uint64_t a, uint64_t b, uint64_t *final);
int						ft_add_and_multiply(int digit, uint64_t *final);
int						ft_atoi_no_overflow(char *nptr, int *final);
int						ft_parse_loop(char *nptr, int *i, uint64_t *nb);
void					ft_parse_sign(char *nptr, int *i, int *sign);
int						handle_exit_error(char **args);
void					print_export_var(char *var);
int						update_existing_var(t_data *data, char *arg,
							char *var_name, size_t len);
void					add_new_var(t_data *data, char *arg, char *equal_pos);
void					close_pipe_fds(int *fds, int index, int count);
void					handle_directory_error(char **argv, t_data *data);
void					handle_command_not_found(char **argv, t_data *data);
void					close_unused_fds(t_exec_context *ctx);
void					handle_empty_argv(char **argv, t_data *data);
void					handle_execve_error(char **argv);
int						is_directory(char *path);
int						count_all_commands(t_cmd *cmd);
int						create_pipes(int *fds, int count);
int						redirect_input_file(t_cmd *cmd, t_redir_in *last);
int						validate_outfile(char *outfile);
void					close_here_fd(int write_fd);
void					process_single_heredoc(t_cmd *cmd, int write_fd,
							int index, t_data *data);
int						setup_heredoc_pipe(int *fds);
pid_t					fork_heredoc_process(void);
void					heredoc_child_process(t_cmd *cmd, int write_fd,
							t_data *data);
void					handle_heredoc_eof(char *delimiter);
void					handle_dollar_expansion(t_expand_ctx *ctx);
int						handle_quotes(char *prompt, int *j, int *quote_state);
int						handle_redirections(t_token_ctx *ctx);
int						handle_special_chars(t_token_ctx *ctx);
void					print_tokens(t_token *head);
void					init_tokenize_vars(t_tokenize_vars *vars,
							t_token **head);
int						process_token_char(char *prompt, t_data *data,
							t_tokenize_vars *vars);
int						finalize_tokenize(t_tokenize_vars *vars);
void					init_first_arg(t_cmd *cmd, t_token *token);
void					add_arg_to_existing(t_cmd *cmd, t_token *token);
int						init_exit_data(t_data *data);
int						init_default_env(t_data *data);
int						init_env_data(t_data *data, char **envp);
char					**fill_argv(t_cmd *cmd, int count);
int						count_args(char **args);
int						handle_redirection(t_cmd *current_cmd, t_token **token);
void					process_token(t_cmd **current_cmd, t_token **token);
void					handle_redout_cmd(t_cmd *cmd, t_token *token);
void					handle_pipe_cmd(t_cmd **current_cmd);
void					handle_append_cmd(t_cmd *cmd, t_token *token);
void					handle_word_cmd(t_cmd *cmd, t_token *token);
char					**create_default_env(void);
void					handle_builtin_execution(char **argv, t_data *data);
int						is_directory_path(char *cmd);
void					handle_command_execution(char **argv, t_data *data);
void					clean_before_token(t_token *token_to_clean,
							t_token *token_final);
void					clean_before_cmd(t_cmd *cmd_to_clean, t_cmd *cmd_final);
void					setup_signals_parent(void);
void					setup_signals_child(void);
void					restore_signals(void);
void					handle_wait_status(t_wait_ctx *ctx);
void					wait_for_children(int count, pid_t last_pid,
							t_data *data);
void					cleanup_heredocs(t_cmd *head);
pid_t					fork_and_execute(t_cmd *cmd, t_exec_context *ctx,
							t_data *data, t_token *tokens);
int						setup_execution_context(t_cmd *cmd, t_data *data,
							int **fds);
void					execute_single_builtin(t_cmd *cmd, t_exec_context *ctx,
							t_data *data, char **argv);
void					execute_pipeline(t_cmd *cmd, t_pipeline_ctx *ctx);
void					reset_fds(int stdin_fd, int stdout_fd);
int						handle_redirect(t_redir_in *last);
pid_t					execute_pipeline_loop(t_cmd *current,
							t_pipeline_ctx *ctx);
int						execute_single_command_if_needed(t_cmd *cmd,
							t_pipeline_ctx *ctx);
int						ft_array_len(char **array);
void					remove_export_only_entry(t_data *data,
							const char *var_name);
int						is_var_in_array(char **array, const char *var_name);

#endif

SRCS_PATH += ./srcs/cmd/
SRCS += execute_cmd.c
SRCS += ftbuiltin_cd.c
SRCS += ftbuiltin_dma_db.c
SRCS += ftbuiltin_echo.c
SRCS += ftbuiltin_env.c
SRCS += ftbuiltin_exit.c
SRCS += ftbuiltin_export_noarg.c
SRCS += ftbuiltin_export.c
SRCS += ftbuiltin_pwd.c
SRCS += ftbuiltin_unset.c

SRCS_PATH += ./srcs/env/
SRCS += change_local_variables.c
SRCS += env_lstaddback.c
SRCS += format_environment.c
SRCS += ft_free_environment.c
SRCS += get_environnement.c
SRCS += get_value_by_key.c
SRCS += path.c
SRCS += replace_key_by_value.c
SRCS += update_environment.c
SRCS += update_local_variables.c

SRCS_PATH += ./srcs/one_time/
SRCS += closing.c
SRCS += init_logs.c
SRCS += opening.c
SRCS += write_to_logfile.c

SRCS_PATH += ./srcs/parsing/
SRCS += parse_args.c
SRCS += parse_builtin.c
SRCS += parse_cmd.c
SRCS += parse_heredoc_limiter.c
SRCS += parsing.c
SRCS += quotes_interpretation.c
SRCS += split_cmds.c

SRCS_PATH += ./srcs/pipex/
SRCS += action_on_files.c
SRCS += check_exit_codes.c
SRCS += error_on_open.c
SRCS += exit_child.c
SRCS += first_child.c
SRCS += ft_free_cmds_parsed.c
SRCS += get_cmds_parsed.c
SRCS += io_open_fds.c
SRCS += last_child.c
SRCS += middle_child.c
SRCS += open_fds.c
SRCS += pipex.c

SRCS_PATH += ./srcs/redirections/
SRCS += ft_free_redirect.c
SRCS += heredoc.c
SRCS += new_redirection.c
SRCS += redirection_check_syntax.c
SRCS += redirections_lst.c
SRCS += redirections.c

SRCS_PATH += ./srcs/routine
SRCS += minishell.c
SRCS += new_prompt.c
SRCS += rm_heredoc_files.c
SRCS += use_argv.c

SRCS_PATH += ./srcs/signals
SRCS += default_signal.c
SRCS += cmd_signal.c
SRCS += heredoc_signal.c

SRCS_PATH += ./srcs/utils/
SRCS += event.c
SRCS += exit_erralloc.c
SRCS += free_log_files.c
SRCS += ft_syntaxerror.c
SRCS += minishell_error.c
SRCS += remove_quote_token_line.c
SRCS += saved_environment.c
SRCS += use_readline.c

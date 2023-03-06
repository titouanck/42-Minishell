SRCS += execute_cmd.c
SRCS += ftbuiltin_cd.c
SRCS += ftbuiltin_echo.c
SRCS += ftbuiltin_env.c
SRCS += ftbuiltin_exit.c
SRCS += ftbuiltin_export.c
SRCS += ftbuiltin_pwd.c
SRCS += ftbuiltin_unset.c

SRCS += change_local_variables.c
SRCS += environment.c
SRCS += get_environnement.c
SRCS += path.c
SRCS += replace_key_by_value.c

SRCS += closing.c
SRCS += opening.c
SRCS += signal.c

SRCS += parse_args.c
SRCS += parse_builtin.c
SRCS += parse_cmd.c
SRCS += parsing.c
SRCS += quotes_interpretation.c
SRCS += redirections.c
SRCS += split_cmds.c

SRCS += first_child.c
SRCS += io_open_fds.c
SRCS += last_child.c
SRCS += middle_child.c
SRCS += pipex.c


SRCS += minishell.c

INC = -I inc/ -I libft/
LIBS = -L libft -lft -lreadline

SRCS_PATH += ./srcs/
SRCS_PATH += ./srcs/cmd/
SRCS_PATH += ./srcs/env/
SRCS_PATH += ./srcs/one-time/
SRCS_PATH += ./srcs/parsing/
SRCS_PATH += ./srcs/pipex/

OBJS_PATH += ./objs	


OBJS = ${patsubst %.c, ${OBJS_PATH}/%.o, ${SRCS}}

vpath %.c ${SRCS_PATH}

# CFLAGS =  -Wall -Wextra -Werror 

NAME = minishell

all:	
		+$(MAKE) -C libft		
		cc -o ${NAME} -g ${CFLAGS} srcs/minishell.c srcs/*/*.c ${INC} ${LIBS}

${OBJS}: ${OBJS_PATH}/%.o: %.c Makefile minishell.h
	@	$(MAKE) --no-print-directory -s -C libft
# @	$(MAKE) --no-print-directory -s -C pipex
	@	mkdir -p ${OBJS_PATH}
	@	$(COLORCOMPIL)
	@	${CC} ${CFLAGS} -c $< -o $@ ${HEAD_PATH}


run:	all
		clear
	@	./${NAME}

valgrind:	all
			clear
			valgrind --track-fds=yes --suppressions=assets/ignore_readline_leaks.supp --leak-check=full --show-leak-kinds=all ./${NAME}
#			valgrind --suppressions=ignore_readline_leaks.supp --track-fds=yes --leak-check=full --show-leak-kinds=all ./${NAME}

clean:	
	@	+$(MAKE) -C libft clean

fclean:	clean;
	@	+$(MAKE) -C libft fclean
	@	rm -f ${NAME}

re:	fclean all

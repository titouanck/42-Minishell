SHELL = /bin/bash


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

INC = -I inc/ -I libft/inc
LIBS = -L libft -lft -lreadline

SRCS_PATH += ./srcs/
SRCS_PATH += ./srcs/cmd/
SRCS_PATH += ./srcs/env/
SRCS_PATH += ./srcs/one-time/
SRCS_PATH += ./srcs/parsing/
SRCS_PATH += ./srcs/pipex/


OBJS = ${patsubst %.c, ${OBJS_PATH}/%.o, ${SRCS}}
OBJS_PATH = ./objs/

CC = cc

vpath %.c ${SRCS_PATH}



# CFLAGS +=  -Wall -Wextra -Werror 
CFLAGS +=   


NAME = minishell

all: ${NAME}

${OBJS}: ${OBJS_PATH}/%.o: %.c Makefile
	@	$(MAKE) --no-print-directory -s -C libft
	@	mkdir -p ${OBJS_PATH}
	@	$(COLORCOMPIL)
	@	${CC} ${CFLAGS} -c $< -o $@ ${INC}

${NAME}:  ${OBJS}
	@	${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS} ${INC}	
	@	echo -ne "\r\033[2K" $(LIGHTGREEN) "→ $(NAME) OK!\n"$(NC)

run:	all
		clear
	@	./${NAME}

valgrind:	all
			clear
			valgrind --track-fds=yes --suppressions=assets/ignore_readline_leaks.supp --leak-check=full --show-leak-kinds=all ./${NAME}

clean:	
	@	+$(MAKE) --no-print-directory -s -C libft clean
	@	echo -ne "\r\033[2K" $(GREEN) "\t$(NAME) cleaned\n"$(NC)
	@	rm -rf ${OBJS_PATH}


fclean:	clean;
	@	+$(MAKE) --no-print-directory -s -C libft fclean
	@	rm -f ${NAME}

re:	fclean all



NOCOLOR='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
LIGHTGRAY='\033[0;37m'

DARKGRAY='\033[1;30m'
LIGHTRED='\033[1;31m'
LIGHTGREEN='\033[1;32m'
LIGHTBLUE='\033[1;34m'
LIGHTPURPLE='\033[1;35m'
LIGHTCYAN='\033[1;36m'
YELLOW='\033[1;33m'
WHITE='\033[1;37m'


ifndef COLORCOMPIL
COLORCOMPIL = \
	if [ "$(shell test $P -lt 33; echo $$?)" = "0" ]; then \
    	echo -ne "\r\033[2K" $(LIGHTRED) "[$(P)%] "$(DARKGRAY) "Compiling MiniShell" $(WHITE) "$<"; \
	else \
		if [ "$(shell test $P -lt 66; echo $$?)" = "0" ]; then \
    		echo -ne "\r\033[2K" $(YELLOW) "[$(P)%]" $(DARKGRAY) "Compiling MiniShell" $(WHITE) "$<"; \
		else \
       		echo -ne "\r\033[2K" $(LIGHTGREEN) "[$(P)%]" $(DARKGRAY) "Compiling MiniShell" $(WHITE) "$<"; \
		fi \
	fi
T := $(words $(SRCS))
N := x
C = $(words $N)$(eval N := x $N)
P = `expr $C '*' 100 / $T / 5`
endif	

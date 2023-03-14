TERM_WIDTH := $(shell tput cols)
SHELL = /bin/bash

SRCS += execute_cmd.c
SRCS += ftbuiltin_cd.c
SRCS += ftbuiltin_echo.c
SRCS += ftbuiltin_env.c
SRCS += ftbuiltin_exit.c
SRCS += ftbuiltin_export.c
SRCS += ftbuiltin_pwd.c
SRCS += ftbuiltin_unset.c
SRCS += ftbuiltin_dma_db.c

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

SRCS += ft_syntaxerror.c

SRCS += minishell.c

INC = -I inc/ -I libft/inc
LIBS = -L libft -lft -lreadline

SRCS_PATH += ./srcs/
SRCS_PATH += ./srcs/cmd/
SRCS_PATH += ./srcs/env/
SRCS_PATH += ./srcs/one-time/
SRCS_PATH += ./srcs/parsing/
SRCS_PATH += ./srcs/pipex/
SRCS_PATH += ./srcs/utils/


OBJS = ${patsubst %.c, ${OBJS_PATH}/%.o, ${SRCS}}
OBJS_PATH = ./objs/

CC = cc

vpath %.c ${SRCS_PATH}

# CFLAGS +=  -Wall -Wextra -Werror 
CFLAGS += -g

NAME = minishell

NC='\033[0m'
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

default:	help
	@	${MAKE} --no-print-directory ${NAME}

all: ${NAME}

ifeq ($(shell test $(TERM_WIDTH) -gt 50; echo $$?), 0)
help:
	@	echo -ne "\r\033[2K" $(WHITE) "----------------------------------------------"${NC}"\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "all       →  "${NC}"Compile the program.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "clean     →  "${NC}"Removes temporary files.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "fclean    →  "${NC}"Deletes all generated files.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "re        →  "${NC}"Rebuilds the project.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "run       →  "${NC}"Compile and executes the program.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "valgrind  →  "${NC}"Check for memory leaks.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "log       →  "${NC}"Creates log file.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE) "libft     →  "${NC}"Rebuilds libft.\n"
	@	echo -ne "\r\033[2K" $(WHITE) "----------------------------------------------"${NC}"\n"
else
help:
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"all\n"${NC}" Compile the program.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"clean\n"${NC}" Removes temporary files.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"fclean\n"${NC}" Deletes all generated files.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"re\n"${NC}" Rebuilds the project.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"run\n"${NC}" Compile and executes the program.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"valgrind\n"${NC}" Check for memory leaks.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"log\n"${NC}" Creates log file.\n"
	@	echo -ne "\r\033[2K" $(LIGHTPURPLE)"libft\n"${NC}" Rebuilds libft.\n"
endif

${OBJS}: ${OBJS_PATH}/%.o: %.c Makefile inc/minishell.h libft/srcs/*/*.c
	@	$(MAKE) --no-print-directory -s -C libft
	@	mkdir -p ${OBJS_PATH}
	@	$(COLORCOMPIL)
	@	${CC} ${CFLAGS} -c $< -o $@ ${INC}

${NAME}:  ${OBJS}
	@	-mkdir -p assets
	@	${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIBS} ${INC}	
	@	echo -ne "\r\033[2K" $(LIGHTGREEN) "→ $(NAME) OK!\n"$(NC)

clean:	
	@	+$(MAKE) --no-print-directory -s -C libft clean
	@	echo -ne "\r\033[2K" $(LIGHTGREEN) "→ $(NAME) cleaned\n"$(NC)
	@	rm -rf ${OBJS_PATH}


fclean:	clean;
	@	+$(MAKE) --no-print-directory -s -C libft fclean
	@	rm -f ${NAME} assets/minishell.log
	@	rm -rf .minishell-logs assets
	@	git clean -f
	@	rm -f /tmp/minishell-heredoc_*

re:	fclean ${NAME}

run:	${NAME}
	@	clear
	@	-./${NAME}
	@	${MAKE} --no-print-directory log

valgrind:	${NAME}
	@	echo -ne \
		"{\n" \
		"	ignore_readline_leaks\n" \
		"	Memcheck:Leak\n" \
		"	...\n" \
		"	fun:readline\n" \
		"}\n" \
		"{\n" \
		"	ignore_readline_leaks\n" \
		"	Memcheck:Leak\n" \
		"	...\n" \
		"	fun:add_history\n" \
		"}\n" > assets/ignore_readline_leaks.supp
	@		echo -ne "\r\033[2K"$(LIGHTBLUE)"Valgrind ./${NAME}: \n--track-fds=yes \n--suppressions=assets/ignore_readline_leaks.supp \n--leak-check=full --show-leak-kinds=all"$(NC)"\n"
	@		-valgrind --track-fds=yes --suppressions=assets/ignore_readline_leaks.supp --leak-check=full --show-leak-kinds=all ./${NAME}
	@		${MAKE} --no-print-directory log

log:
	@	if [ -d .minishell-logs ]; then \
			rm -f assets/minishell.log; \
			find .minishell-logs -type f -printf "%T@ %p\n" | sort | cut -d' ' -f2- | xargs awk 'FNR==1 && NR!=1 {print "\n----------------------------------------------\n"}{print}' - > assets/minishell.log; \
			rm -rf .minishell-logs; \
			echo -ne "\r\033[2K" $(LIGHTGREEN) "→ assets/$(NAME).log OK!"$(NC)"\n"; \
		elif [ -f assets/minishell.log ]; then \
			echo -ne "\r\033[2K" $(ORANGE) "→ Unable to update assets/$(NAME).log, already up-to-date ?"$(NC)"\n"; \
		else \
			echo -ne "\r\033[2K" $(LIGHTRED) "→ Unable to generate assets/$(NAME).log :("$(NC)"\n"; \
		fi

libft:
	@	+$(MAKE) --no-print-directory -s -C libft re

ifndef COLORCOMPIL
ifeq ($(shell test $(TERM_WIDTH) -gt 69; echo $$?), 0)
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
else
COLORCOMPIL = \
	if [ "$(shell test $P -lt 33; echo $$?)" = "0" ]; then \
    	echo -ne "\r\033[2K" $(LIGHTRED) "[$(P)%] "$(DARKGRAY) "Compiling MiniShell"; \
	else \
		if [ "$(shell test $P -lt 66; echo $$?)" = "0" ]; then \
    		echo -ne "\r\033[2K" $(YELLOW) "[$(P)%]" $(DARKGRAY) "Compiling MiniShell"; \
		else \
       		echo -ne "\r\033[2K" $(LIGHTGREEN) "[$(P)%]" $(DARKGRAY) "Compiling MiniShell"; \
		fi \
	fi
endif
T := $(words $(SRCS))
N := x
C = $(words $N)$(eval N := x $N)
P = `expr $C '*' 100 / $T / 5`
endif

.PHONY: libft

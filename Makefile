include srcs/make/srcs.mk
include srcs/make/colors.mk

TERM_WIDTH := $(shell tput cols)
SHELL = /bin/bash

vpath %.c ${SRCS_PATH}

INC = -I inc/ -I libft/inc
LIBS = -L libft -lft -lreadline

OBJS = ${patsubst %.c, ${OBJS_PATH}/%.o, ${SRCS}}
OBJS_PATH = ./objs/

CC = cc
CFLAGS += -g 
CFLAGS += -Wall -Wextra -Werror 
NAME = minishell

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

${OBJS}: ${OBJS_PATH}/%.o: %.c Makefile inc/minishell.h
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
	@	rm -rf /tmp/.minishell-logs assets
	@	rm -f /tmp/minishell-heredoc_*

re:	fclean ${NAME}

run:	${NAME}
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
	@	if [ -d /tmp/.minishell-logs ]; then \
			rm -f assets/minishell.log; \
			find /tmp/.minishell-logs -type f -printf "%T@ %p\n" | sort | cut -d' ' -f2- | xargs awk 'FNR==1 && NR!=1 {print "\n----------------------------------------------\n"}{print}' - > assets/minishell.log; \
			rm -rf /tmp/.minishell-logs; \
			echo -ne "\r\033[2K" $(LIGHTGREEN) "→ assets/$(NAME).log OK!"$(NC)"\n"; \
		elif [ -f assets/minishell.log ]; then \
			echo -ne "\r\033[2K" $(ORANGE) "→ Unable to update assets/$(NAME).log, already up-to-date ?"$(NC)"\n"; \
		else \
			echo -ne "\r\033[2K" $(LIGHTRED) "→ Unable to generate assets/$(NAME).log :("$(NC)"\n"; \
		fi

libft:
	@	+$(MAKE) --no-print-directory -s -C libft re

include srcs/make/colorcompil.mk

.PHONY: default all clean fclean re run valgrind log libft 

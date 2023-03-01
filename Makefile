INC = -I inc/ -I libft/
LIBS = -L libft -lft -lreadline

# CFLAGS =  -Wall -Wextra -Werror 

NAME = minishell

all:	
		+$(MAKE) -C libft
		cc -o ${NAME} -g ${CFLAGS} srcs/minishell.c srcs/*/*.c ${INC} ${LIBS}

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

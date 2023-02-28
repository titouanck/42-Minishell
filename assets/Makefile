NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra -g -MMD
LFLAGS = -LLibft_vde-leus -lft -lreadline

define SRCS :=
	Main/Main.c
	
	Built_in_functions/Env.c
	Built_in_functions/Environment_utils_1.c
	Built_in_functions/Environment_utils_2.c
	Built_in_functions/Export.c
	Built_in_functions/Unset.c
	Built_in_functions/Directory_PWD_CD.c
	Built_in_functions/Exit_and_Free.c
	Built_in_functions/Echo.c

	Signal_Message/Signal.c
	Signal_Message/Message.c
endef

SRCS_PATH = Srcs
OBJS_PATH = Objs

INCLUDES = -I Includes -I Libft_vde-leus

OBJS = ${patsubst %.c, ${OBJS_PATH}/%.o, ${SRCS}}
DEPS = $(OBJS:.o=.d)

all: ${NAME}

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.c
		mkdir -p ${dir $@}
		${CC} ${CFLAGS} -c $< -o $@ $(INCLUDES)

${NAME}: ${OBJS}
		$(MAKE) -C Libft_vde-leus/ all
		$(CC) -o $(NAME) ${OBJS} $(LFLAGS)

clean:	
		rm -rf $(OBJS_PATH)
		$(MAKE) -C Libft_vde-leus/ clean
		clear

fclean:	clean
		$(MAKE) -C Libft_vde-leus/ fclean
		rm -f ${NAME}
		clear

re:	fclean all

.PHONY: clean fclean re 

-include $(DEPS)
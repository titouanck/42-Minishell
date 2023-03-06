# LIBFT
LIBC =	Libft/ft_atoi.c Libft/ft_bzero.c Libft/ft_calloc.c Libft/ft_isalnum.c Libft/ft_isalpha.c \
		Libft/ft_isascii.c Libft/ft_isdigit.c Libft/ft_isprint.c Libft/ft_memchr.c \
		Libft/ft_memcmp.c Libft/ft_memcpy.c Libft/ft_memmove.c Libft/ft_memset.c Libft/ft_strchr.c \
		Libft/ft_strdup.c Libft/ft_strlcat.c Libft/ft_strlcpy.c Libft/ft_strlen.c Libft/ft_strncmp.c \
		Libft/ft_strnstr.c Libft/ft_strrchr.c Libft/ft_tolower.c Libft/ft_toupper.c \
		Libft/ft_strip.c Libft/ft_strinset.c Libft/ft_iswhitespace.c Libft/ft_strinsert.c Libft/print_tabstr.c Libft/ft_inset.c

ADDITIONAL =	Libft/ft_itoa.c Libft/ft_putchar_fd.c Libft/ft_putendl_fd.c Libft/ft_putnbr_fd.c Libft/ft_putstr_fd.c \
				Libft/ft_split.c Libft/ft_strjoin.c Libft/ft_strmapi.c Libft/ft_strtrim.c Libft/ft_substr.c Libft/ft_striteri.c

BONUS =	Libft/ft_lstadd_back_bonus.c Libft/ft_lstadd_front_bonus.c Libft/ft_lstclear_bonus.c \
		Libft/ft_lstdelone_bonus.c Libft/ft_lstiter_bonus.c Libft/ft_lstlast_bonus.c \
		Libft/ft_lstmap_bonus.c Libft/ft_lstnew_bonus.c Libft/ft_lstsize_bonus.c Libft/ft_swap.c

SRCSLIBFT = ${LIBC} ${ADDITIONAL} ${BONUS}

# FT_PRINTF
SRCSPRINTF =	ft_printf/ft_printf.c ft_printf/ft_putadress.c ft_printf/ft_putchar.c \
				ft_printf/ft_puthexa_lower.c ft_printf/ft_puthexa_upper.c ft_printf/ft_putnbr.c \
				ft_printf/ft_putstr.c ft_printf/ft_putunbr.c

# GET_NEXT_LINE
SRCSGETNEXTLINE = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

# Makefile

SRCS = ${SRCSLIBFT} ${SRCSPRINTF} ${SRCSGETNEXTLINE}

OBJS_PATH = ./obj
OBJS = ${patsubst %.c, ${OBJS_PATH}/%.o, ${SRCS}}

INCLUDE = -I ./inc




LIB = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror -I ./

all: 	${LIB}

${OBJS}: ${OBJS_PATH}/%.o: %.c Makefile
	@	mkdir -p ${OBJS_PATH}
# @	$(COLORCOMPIL)
	@	${CC} ${CFLAGS} -c $< -o $@ ${INCLUDE}

${LIB}:	${OBJS}
	@	ar -rsc ${LIB} ${OBJS}
	@	echo -ne "\r\033[2K" $(LIGHTGREEN) "\t$(LIB) OK"$(NC)"\n""\n"

clean:	
	@	rm -f ${OBJS} ${DEPS}

fclean:	clean;
	@	rm -f ${LIB}

re:	fclean all


.PHONY: all clean fclean re bonus


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


ifndef COLORCOMPIL
COLORCOMPIL = \
	if [ "$(shell test $P -lt 33; echo $$?)" = "0" ]; then \
    	echo -ne "\r\033[2K" $(LIGHTRED) "[$(P)%] "$(DARKGRAY) "Compiling Libft" $(WHITE) "$<"; \
	else \
		if [ "$(shell test $P -lt 66; echo $$?)" = "0" ]; then \
    		echo -ne "\r\033[2K" $(YELLOW) "[$(P)%]" $(DARKGRAY) "Compiling Libft" $(WHITE) "$<"; \
		else \
       		echo -ne "\r\033[2K" $(LIGHTGREEN) "[$(P)%]" $(DARKGRAY) "Compiling Libft" $(WHITE) "$<"; \
		fi \
	fi
T := $(words $(SRCS))
N := x
C = $(words $N)$(eval N := x $N)
P = `expr $C '*' 100 / $T / 5`
endif

SHELL = /bin/bash

# ft_printf

SRCS += ft_printf.c
SRCS += ft_putchar.c
SRCS += ft_puthexa_upper.c
SRCS += ft_putstr.c
SRCS += ft_putadress.c
SRCS += ft_puthexa_lower.c
SRCS += ft_putnbr.c
SRCS += ft_putunbr.c

# get_next_line

SRCS += get_next_line.c
SRCS += get_next_line_utils.c

# libc

SRCS += ft_atoi.c
SRCS += ft_isalpha.c
SRCS += ft_memchr.c
SRCS += ft_memset.c
SRCS += ft_strlcat.c
SRCS += ft_strndup.c
SRCS += ft_toupper.c
SRCS += ft_bzero.c
SRCS += ft_isascii.c
SRCS += ft_memcmp.c
SRCS += ft_strchr.c
SRCS += ft_strlcpy.c
SRCS += ft_strnstr.c
SRCS += ft_calloc.c
SRCS += ft_isdigit.c
SRCS += ft_memcpy.c
SRCS += ft_strcmp.c
SRCS += ft_strlen.c
SRCS += ft_strrchr.c
SRCS += ft_isalnum.c
SRCS += ft_isprint.c
SRCS += ft_memmove.c
SRCS += ft_strdup.c
SRCS += ft_strncmp.c
SRCS += ft_tolower.c

# additional

SRCS += ft_itoa.c
SRCS += ft_putendl_fd.c
SRCS += ft_putstr_fd.c
SRCS += ft_striteri.c
SRCS += ft_strmapi.c
SRCS += ft_substr.c
SRCS += ft_putchar_fd.c
SRCS += ft_putnbr_fd.c
SRCS += ft_split.c
SRCS += ft_strjoin.c
SRCS += ft_strtrim.c

# mine

SRCS += ft_countc.c
SRCS += ft_freetab.c
SRCS += ft_printtab.c
SRCS += ft_strchrset.c
SRCS += ft_strrjoin.c
SRCS += ft_inset.c
SRCS += ft_strinset.c
SRCS += ft_iswhitespace.c
SRCS += ft_strip.c
SRCS += ft_swap.c
SRCS += ft_isnotnull.c

# dynamic_memory_address_db

SRCS += dynamic_memory_address_db.c
SRCS += _addressdb_functions.c
SRCS += db_malloc.c
SRCS += db_calloc.c
SRCS += db_strdup.c
SRCS += db_strndup.c
SRCS += db_free.c

INC = -I inc

SRCS_PATH += srcs/ft_printf
SRCS_PATH += srcs/get_next_line
SRCS_PATH += srcs/libc
SRCS_PATH += srcs/additional
SRCS_PATH += srcs/mine
SRCS_PATH += srcs/dynamic_memory_address_db

OBJS = ${patsubst %.c, ${OBJS_PATH}/%.o, ${SRCS}}
OBJS_PATH = ./objs/

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

vpath %.c ${SRCS_PATH}

NAME = libft.a
EXEC = xlibft

all:	${NAME}

${OBJS}: ${OBJS_PATH}/%.o: %.c Makefile
	@	mkdir -p ${OBJS_PATH}
	@	$(COLORCOMPIL)
	@	${CC} ${CFLAGS} -c $< -o $@ ${INC}

${NAME}:  ${OBJS}
	@	ar -rsc ${NAME} ${OBJS}
	@	echo -ne "\r\033[2K" $(LIGHTGREEN) "→ $(NAME) OK!\n"$(NC)

exec:	${OBJS}
	@	${CC} ${CFLAGS} -o ${EXEC} ${OBJS} ${INC}
	@	echo -ne "\r\033[2K" $(LIGHTGREEN) "→ $(EXEC) OK!\n"$(NC)

clean:	
	@	rm -rf ${OBJS_PATH}

fclean:	clean;
	@	rm -f ${NAME} ${EXEC}

re:	fclean all

# Colors

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
    	echo -ne "\r\033[2K" $(LIGHTRED) "[$(P)%] "$(DARKGRAY) "Compiling ${NAME}" $(WHITE) "$<"; \
	else \
		if [ "$(shell test $P -lt 66; echo $$?)" = "0" ]; then \
    		echo -ne "\r\033[2K" $(YELLOW) "[$(P)%]" $(DARKGRAY) "Compiling ${NAME}" $(WHITE) "$<"; \
		else \
       		echo -ne "\r\033[2K" $(LIGHTGREEN) "[$(P)%]" $(DARKGRAY) "Compiling ${NAME}" $(WHITE) "$<"; \
		fi \
	fi
T := $(words $(SRCS))
N := x
C = $(words $N)$(eval N := x $N)
P = `expr $C '*' 100 / $T / 5`
endif	

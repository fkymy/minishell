# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/06/27 20:32:45 by yufukuya          #+#    #+#              #
#    Updated: 2021/02/08 17:36:11 by tayamamo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := libft.a
CC := gcc
CFLAGS := -Wall -Werror -Wextra

includes := .

mandatory := ft_memset.c \
			 ft_bzero.c \
			 ft_memcpy.c \
			 ft_memccpy.c \
			 ft_memmove.c \
			 ft_memchr.c \
			 ft_memcmp.c \
			 ft_strlen.c \
			 ft_strslen.c \
			 ft_strlcpy.c \
			 ft_strlcat.c \
			 ft_strchr.c \
			 ft_strrchr.c \
			 ft_lastchr.c \
			 ft_strnstr.c \
			 ft_strncmp.c \
			 ft_atoi.c \
			 ft_atol.c \
			 ft_isalpha.c \
			 ft_isdigit.c \
			 ft_isalnum.c \
			 ft_isascii.c \
			 ft_isprint.c \
			 ft_isspace.c \
			 ft_isspaces.c \
			 ft_toupper.c \
			 ft_tolower.c \
			 ft_calloc.c \
			 ft_strdup.c \
			 ft_strcmp.c

mandatory += ft_substr.c \
			 ft_strjoin.c \
			 ft_strjoin_chr.c \
			 ft_strjoin_chr_free.c \
			 ft_strtrim.c \
			 ft_split.c \
			 ft_itoa.c \
			 ft_strmapi.c \
			 ft_strsort.c \
			 ft_stradd.c \
			 ft_putchar_fd.c \
			 ft_putstr_fd.c \
			 ft_putendl_fd.c \
			 ft_putnbr_fd.c

bonus := ft_lstnew.c \
		 ft_lstadd_front.c \
		 ft_lstsize.c \
		 ft_lstlast.c \
		 ft_lstadd_back.c \
		 ft_lstdelone.c \
		 ft_lstclear.c \
		 ft_lstiter.c \
		 ft_lstmap.c \
		 get_next_line.c \
		 ft_free_null.c \
		 ft_strjoin_free.c \
		 ft_split_free_null.c \
		 ft_split_join_chr.c \
		 ft_split_skip_str.c \
		 ft_teardown.c

objects := $(mandatory:.c=.o)

objects_bonus := $(bonus:.c=.o)

.PHONY: all
all: $(NAME)

$(NAME): $(objects)
	@ar -r $@ $^

$(objects): %.o: %.c
	@$(CC) -c $(CFLAGS) -I$(includes) $< -o $@

.PHONY: bonus
bonus: $(objects_bonus)
	@ar -r $(NAME) $^

$(objects_bonus): %.o: %.c
	@$(CC) -c $(CFLAGS) -I$(includes) $< -o $@

.PHONY: re fclean clean
re: fclean all
fclean: clean ; -rm -f $(NAME)
clean: ; -rm -f *.o

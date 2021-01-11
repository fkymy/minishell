
NAME := minishell
CC := gcc
CFLAGS := -g -Wall -Wextra -Werror

LIBFT_DIR := libft
LIBFT := $(LIBFT_DIR)/libft.a

INCLUDES := .
SRCS := main.c \
		vector_string.c \
		token.c
		command.c \

OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) -I$(INCLUDES) -I$(LIBFT_DIR) $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)
	make -C $(LIBFT_DIR) bonus

re: fclean all

fclean: clean
	-rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

clean:
	-rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

.PHONY: all re fclean clean bonus

debug:


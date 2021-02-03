# Makefile for minishell, updated Tue Feb  2 18:28:53 JST 2021

SRC := command.c commandline.c env.c ft_exit.c ft_export.c ft_unset.c main.c parse.c redir.c shell_utils.c signal.c token.c vector.c wordexp.c wordexp_quote.c wordexp_var.c

OBJ := command.o commandline.o env.o ft_exit.o ft_export.o ft_unset.o main.o parse.o redir.o shell_utils.o signal.o token.o vector.o wordexp.o wordexp_quote.o wordexp_var.o

HDR := minishell.h

# DO NOT ADD OR MODIFY ANY LINES ABOVE THIS -- run 'make source' to add files

NAME := minishell

CC := gcc
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -g -fsanitize=address

BINDIR := /usr/local/bin

LIBS := -Llibft -lft

LIBFT_DIR := libft
LIBFT := $(LIBFT_DIR)/libft.a

RESET		= \033[0m
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
MAGENT		= \033[0;35m
CYAAN		= \033[0;36m
WHITE		= \033[0;37m
B_RESET		= \033[0;49m
B_YELLOW	= \033[0;43m
B_CYAAN		= \033[0;46m
BOLD		= \033[1m
UNDER_LINE	= \033[4m

.PHONY: all re fclean clean install source lint test
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
	@echo "\n$(GREEN)$(BOLD)$(UNDER_LINE)Compiled Successfully.$(RESET)\n"

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@make -C $(LIBFT_DIR) bonus

re: fclean all

fclean: clean
	-rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

clean:
	-rm -f Makefile.bak $(OBJ)
	make clean -C $(LIBFT_DIR)

install: $(NAME)
	cp $(NAME) $(BINDIR)
	chmod 755 $(BINDIR)/$(NAME)

source:
	@mv Makefile Makefile.bak
	@echo "# Makefile for minishell, updated `date`" > Makefile
	@echo '' >> Makefile
	@echo SRC := `ls *.c` >> Makefile
	@echo '' >> Makefile
	@echo OBJ := `ls *.c | sed s/c$$/o/` >> Makefile
	@echo '' >> Makefile
	@echo HDR := `ls *.h` >> Makefile
	@echo '' >> Makefile
	@sed -n -e '/^# DO NOT ADD OR MODIFY/,$$p' < Makefile.bak >> Makefile

test:
	@cd test && bash test.sh

lint:
	~/.norminette/norminette.rb $(SRC) $(HDR)

# Makefile for minishell, updated Fri Feb  5 17:41:24 JST 2021

SRC := builtin.c command.c commandline.c env.c execute.c ft_cd.c ft_echo.c ft_exit.c ft_export.c ft_pwd.c ft_unset.c main.c parse.c path.c path_utils.c pipeline.c redir.c shell_utils.c signal.c token.c vector.c wordexp.c wordexp_quote.c wordexp_var.c

OBJ := builtin.o command.o commandline.o env.o execute.o ft_cd.o ft_echo.o ft_exit.o ft_export.o ft_pwd.o ft_unset.o main.o parse.o path.o path_utils.o pipeline.o redir.o shell_utils.o signal.o token.o vector.o wordexp.o wordexp_quote.o wordexp_var.o

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

.PHONY: all re fclean clean install source lint test
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
	@echo "\nCompiled Successfully.\n"

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

# Makefile for minishell, updated Fri Jan 29 18:23:20 JST 2021

SRC := command.c get_next_commandline.c main.c redir.c signal.c token.c vector_string.c

OBJ := command.o get_next_commandline.o  main.o redir.o signal.o token.o vector_string.o

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

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/24 07:25:17 by rosousa-          #+#    #+#              #
#    Updated: 2026/08/27 21:05:44 by rodrigo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = cub3d
CC          = cc -g
CFLAGS      = -Wall -Wextra -Werror
MLX_DIR     = ./minilibx-linux
MLX_LNK     = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

SRC         = main.c \
              cub3d.c \
              err.c \
#               render.c \
#               math_utils.c \
#               events.c \
#               string_utils.c \
#               parse.c \

OBJ         = $(SRC:.c=.o)

GREEN       = \033[0;32m
RESET       = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(OBJ) $(MLX_LNK) -o $(NAME)
	@echo "$(GREEN)Cub3D compilado com sucesso!$(RESET)"

%.o: %.c
	$(CC) $(CFLAGS) -I$(MLX_DIR) -c $< -o $@

clean:
	@make -C $(MLX_DIR) clean
	rm -f $(OBJ)
	@echo "Objetos removidos."

fclean: clean
	rm -f $(NAME)
	make -C $(MLX_DIR) clean
	@echo "Executável removido."

re: fclean all

.PHONY: all clean fclean re
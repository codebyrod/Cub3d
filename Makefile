# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/24 07:25:17 by rosousa-          #+#    #+#              #
#    Updated: 2026/11/25 18:18:55 by rosousa-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = cub3D
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g
DEPFLAGS    = -MMD -MP
INCLUDES	= -Iincludes -Ilibft -I$(MLX_DIR)

#Directories
GAME_DIR     = game
PARSING_DIR  = parsing
LIBFT_DIR    = libft
MLX_DIR      = minilibx-linux

#libraries
LIBFT       = $(LIBFT_DIR)/libft.a
MLX         = $(MLX_DIR)/libmlx.a
MLX_LNK     = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# Sources 
ROOT_SRC    = main.c

PARSING_SRC	= parse.c \
                parse_config.c \
                parse_free.c \
                parse_map.c \
                parse_utils.c \
                parse_values.c \
                sufix_verification.c \
                validation_map.c \
                validation_size.c

#Game sources
GAME_SRC    = camera.c \
                clear.c \
                clear_aux.c \
                config_bridge_game.c \
                cub3d.c \
                data_init.c \
                err_init_cub.c \
                events.c \
                game_render.c \
                map.c \
                movement.c \
                player.c \
                ray_utils.c \
                ray.c \
                textures.c \
                wall_utils.c \
                wall.c


#Objects
ROOT_OBJS         = $(addprefix objs/, $(ROOT_SRC:.c=.o))
GAME_OBJS         = $(addprefix $(GAME_DIR)/objs/, $(GAME_SRC:.c=.o))
PARSING_OBJS      = $(addprefix $(PARSING_DIR)/objs/, $(PARSING_SRC:.c=.o))


OBJS        = $(ROOT_OBJS) $(GAME_OBJS) $(PARSING_OBJS)
DEPS	    = $(OBJS:.o=.d)
OBJ_DIR	    = objs $(GAME_DIR)/objs $(PARSING_DIR)/objs

GREEN       = \033[0;32m
RESET       = \033[0m

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LNK) -o $(NAME)
	@echo "$(GREEN)Cub3D compilado com sucesso!$(RESET)"

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

#root object
objs/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

#Game object
$(GAME_DIR)/objs/%.o: $(GAME_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

#Parsing object
$(PARSING_DIR)/objs/%.o: $(PARSING_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@


#CLEANUP
clean:

	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) clean
	rm -rf $(OBJ_DIR)
	@echo "Pastas objs removidas."

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	@echo "Executável removido."

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
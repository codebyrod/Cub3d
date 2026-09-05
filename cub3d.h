#ifndef CUB3D_H
 # define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include "minilibx-linux/mlx.h"
# include "X11/keysym.h"
# include "X11/X.h"
# include "includes/includes.h"

# define HEIGHT 400
# define WIDTH 400
# define TILE_SIZE 10
# define RED	0X9e1c3c
# define BLUE	0X124ac4

enum status_malloc
{
	MALLOC_SUCESS,
	MALLOC_ERROR
};

typedef struct s_img
{
	void	*img_ptr;
	char	*img_pixels_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		endian;
	int		size_len;
}	t_img;

typedef struct s_pxl
{
	int	pxl_x;
	int	pxl_y;	
}	t_pxl;

typedef struct map
{
	char	**map;
	int		map_width;
	int		map_height;
}	t_map;


typedef struct s_game
{
	void	*connection;
	void	*window;
	char	*name_wd;
	int		hook;
	t_img	img;
	t_pxl	pxl;
	t_map	map;


}	t_game;

// FUNÇÕES PRINCIPAIS
int		cub_init(t_game *game);
void	end_connection(t_game *game);

//FUNÇÕES DE LIMPEZA DA CUB_INIT
void 	err_init_cub(t_game *game, char *str);
void	clear_connection(t_game *game);
void	clear_window(t_game *game);
void	clear_img_ptr(t_game *game);
void	clear_img_pixel(t_game *game);

//imagens 2d
// void put_img(t_game *game);
void	game_render(t_game *game);
void	handle_pixel(t_game *game);
void	my_pixel_put(t_img *img, int x, int y, int color);

//map
void	map_render(t_game *game);

//player
void	player_render(t_game *game);

//events
int		handle_movement(t_game *game);
int		close_handler(t_game *game);
void	events_init(t_game *game);

#endif

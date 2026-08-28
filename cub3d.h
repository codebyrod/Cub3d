#ifndef CUB3D_H
 # define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
#include <unistd.h>
# include <math.h>
# include "minilibx-linux/mlx.h"
# include "X11/keysym.h"
# include "X11/X.h"
# include "includes/includes.h"



# define HEIGHT 800
# define WIDTH 800

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

typedef struct s_coord
{
	int	x;
	int	y;	
}	t_coord;

typedef struct s_game
{
	void	*connection;
	void	*window;
	char	*name_wd;
	int		hook;
	t_img	img;
	t_coord	coord;

}	t_game;

int		cub_init(t_game game);
void 	err_init_cub(t_game game, char *str);
void	clear_connection(t_game game);
void	clear_window(t_game game);
void	clear_img_ptr(t_game game);
void	clear_img_pixel(t_game game);


#endif

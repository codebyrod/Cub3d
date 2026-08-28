#ifndef CUB3D_H
 # define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "minilibx-linux/mlx.h"
# include "X11/keysym.h"
# include "X11/X.h"

# define MALLOC_ERROR 1
# define HEIGHT 800
# define WIDTH 800

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

int	cub_init(t_game game);

#endif

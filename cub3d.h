#ifndef CUB3D_H
 # define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
# include "minilibx-linux/mlx.h"
# include "X11/keysym.h"
# include "X11/X.h"

typedef struct s_game
{
	void	*connection;
	void	*window;
	char	*name_wd;
	t_img	img;
	t_coord	coord;

}	t_game;

typedef struct s_img
{
	void	*img_ptr;
	char	*img_pixels_ptr;
	int		bits_per_pixel;
	int		endian;
	int		line_len;
}	t_img;

typedef struct s_coord
{
	int	x;
	int	y;	
}	t_coord;


#endif

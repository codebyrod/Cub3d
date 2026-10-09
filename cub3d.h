#ifndef CUB3D_H
 # define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <sys/time.h>
# include "minilibx-linux/mlx.h"
# include "X11/keysym.h"
# include "X11/X.h"
# include "includes/includes.h"

# define HEIGHT 400
# define WIDTH 400
# define TILE_SIZE 20
# define PLAYER_SIZE 10
# define SPEED_SEC 3
# define ROT_SPEED_SEC 90
# define RAY_STEP 0.01
# define RAY_MAX_DIST 50.0
# define RED	0xcc3bd1
# define GRAY	0x414755
# define BLUE	0x164cca
# define GREEN	0X168a06
# define YELLOW	0xddb70d
# define VIOLET	0X70034c
# define PINK	0xf3c3f3

typedef enum e_status_malloc
{
	MALLOC_SUCESS,
	MALLOC_ERROR
}	t_status_malloc;

typedef	enum	e_bool
{
	BOOL_FALSE,
	BOOL_TRUE
}	t_bool;

typedef struct s_keys
{
	int		w;
	int		a;
	int		s;
	int		d;
	int		x;
	int		esc;
	int		a_up;
	int		a_right;
	int		a_bottom;
	int		a_left;	
}	t_keys;

typedef struct s_time
{
	double	last_frame_time;
	double	delta_time;
}	t_time;

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

typedef struct s_map
{
	char	**map;
	int		map_width;
	int		map_height;
}	t_map;

typedef enum e_side
{
	SIDE_NONE,
	SIDE_N,
	SIDE_S,
	SIDE_E,
	SIDE_W
}	t_side;

typedef struct s_hit
{
	double	dist;
	t_side	side;
	double	hit_x;
	double	hit_y;
}	t_hit;

typedef struct s_player
{
	int		pos_init_Y;
	int		pos_init_X;
	double	play_x;
	double	play_y;
	double	radius;
	double	delta_x;
	double	delta_y;
	double	magnitude;
	double	dir_x;
	double	dir_y;
	double	angle;
	double	cam_x;
	double	cam_y;
}	t_player;


typedef struct s_game
{
	void		*connection;
	void		*window;
	char		*name_wd;
	int			hook;
	t_img		img;
	t_pxl		pxl;
	t_player 	play;
	t_map		map;
	t_keys		keys;
	t_time		time;
}	t_game;

// FUNÇÕES PRINCIPAIS
int		cub_init(t_game *game);
// void	end_connection(t_game *game);

//FUNÇÕES DE LIMPEZA DA CUB_INIT
void 	err_init_cub(t_game *game, char *str);
void	clear_connection(t_game *game);
void	clear_window(t_game *game);
void	clear_img_ptr(t_game *game);
void	clear_img_pixel(t_game *game);

//imagens 2d
// void put_img(t_game *game);
void	game_render(t_game *game);
void	put_tile(t_game *game, int SIZE, int coord_x, int coord_y, int color);
// void	handle_pixel(t_game *game);
void	my_pixel_put(t_img *img, int x, int y, int color);

//map
void	map_render(t_game *game);

//player
void	player_render(t_game *game);


//events
int		handle_movement(t_game *game);
int		close_handler(t_game *game);
void	events_init(t_game *game);

//data inits
void	data_init(t_game *game);
void	pos_init_player(t_game *game);

//movements
int		handle_movement(t_game *game);


//raycasting
t_hit	cast_ray(t_game *game, double ray_dx, double ray_dy);
void	draw_ray_debug(t_game *game, double ray_dx, double ray_dy);
void	test_ray(t_game *game);


#endif

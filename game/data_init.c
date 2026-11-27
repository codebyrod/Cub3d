/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:55:50 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 16:41:44 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	key_mov_init(t_game *game)
{
	game->keys.w = BOOL_FALSE;
	game->keys.a = BOOL_FALSE;
	game->keys.s = BOOL_FALSE;
	game->keys.d = BOOL_FALSE;
	game->keys.a_up = BOOL_FALSE;
	game->keys.a_right = BOOL_FALSE;
	game->keys.a_left = BOOL_FALSE;
	game->keys.a_bottom = BOOL_FALSE;
	game->keys.x = 0;
	game->keys.esc = 0;
}

static void	set_player_dir(t_game *game, char c)
{
	game->play.dir_x = 0;
	game->play.dir_y = 0;
	if (c == 'N')
		game->play.dir_y = -1;
	else if (c == 'S')
		game->play.dir_y = 1;
	else if (c == 'E')
		game->play.dir_x = 1;
	else if (c == 'W')
		game->play.dir_x = -1;
}

void	player_init(t_game *game)
{
	game->play.play_x += 0.5;
	game->play.play_y += 0.5;
	set_player_dir(game, game->play.view_dir);
	game->play.radius = ((double)PLAYER_SIZE / (double)TILE_SIZE) / 2.0;
	game->play.delta_x = 0;
	game->play.delta_y = 0;
	game->play.magnitude = 0;
	game->play.angle = 0;
	init_camera_plane(game);
}

void	init_camera_plane(t_game *game)
{
	double	plane_length;

	plane_length = tan((FOV_DEG * M_PI / 180.0) / 2.0);
	game->play.cam_x = -game->play.dir_y * plane_length;
	game->play.cam_y = game->play.dir_x * plane_length;
}

void	data_init(t_game *game)
{
	struct timeval	tv;

	key_mov_init(game);
	player_init(game);
	gettimeofday(&tv, NULL);
	game->time.last_frame_time = tv.tv_sec + (tv.tv_usec / 1000000.0);
}

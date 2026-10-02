/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:55:50 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/01 23:59:09 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	key_mov_init(t_game *game)
{
	game->keys.w =	BOOL_FALSE;
	game->keys.a =	BOOL_FALSE;
	game->keys.s =	BOOL_FALSE;
	game->keys.d =	BOOL_FALSE;
	game->keys.a_up = BOOL_FALSE;
	game->keys.a_right = BOOL_FALSE;
	game->keys.a_left =	BOOL_FALSE;
	game->keys.a_bottom = BOOL_FALSE;
	game->keys.x = 0;
	game->keys.esc = 0;
}

void	pos_init_player(t_game *game)
{
	int	i;
	int	j;
	i = 0;
	while (i < game->map.map_width)
	{
		j = 0;
		while (j < game->map.map_height)
		{
			if (game->map.map[j][i] == '0')
			{
				game->play.pos_init_X = i;
				game->play.pos_init_Y = j;
				return ;
			}
			j++;
		}	
		i++;
	}
}

void	player_init(t_game *game)
{
	pos_init_player(game);
	game->play.play_x = game->play.pos_init_X + 0.3;
	game->play.play_y = game->play.pos_init_Y + 0.3;
	// game->play.play_x = game->play.pos_init_X;
	// game->play.play_y = game->play.pos_init_Y;
	game->play.radius = (((double)PLAYER_SIZE * 1.0) / ((double)TILE_SIZE * 1.0) / 2.0);
	game->play.delta_x = 0;
	game->play.delta_y = 0;
	game->play.magnitude = 0;
	game->play.dir_x = 0;
	game->play.dir_y = 0;
	game->play.cam_x = 0;
	game->play.cam_y = 0;
}

void	data_init(t_game *game)
{
	struct timeval tv;

	key_mov_init(game);
	player_init(game);
	gettimeofday(&tv, NULL);
	game->time.last_frame_time = tv.tv_sec + (tv.tv_usec / 1000000.0);
}

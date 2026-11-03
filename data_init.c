/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:55:50 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/02 21:36:25 by rodrigo          ###   ########.fr       */
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

	// printf("MAP POINTER: %p\n", (void *)game->map.map);
	// printf("MAP WIDTH: %d\n", game->map.map_width);
	// printf("MAP HEIGHT: %d\n", game->map.map_height);
	
	i = 0;
	while (i < game->map.map_width)
	{
		j = 0;
		while (j < game->map.map_height)
		{
			// printf("Acessando [%d][%d]\n", j, i);
			// printf("Linha: %s\n", game->map.map[j]);
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
	game->play.dir_y = 1;

	//Esse angle precisa ser em radianos
	game->play.angle = 0;
	init_camera_plane(game);
}


// o plano da camera precisa ser perpendicular ao vetor de direção. o comprimento determina o campo de visão horizontal.
void	init_camera_plane(t_game *game)
{
	double	plane_length;

	// Calcula o comprimento necessário para o FOV escolhido.
	plane_length = tan((FOV_DEG * M_PI / 180.0) / 2.0);

	// Cria um vetor perpendicular à direção do jogador.
	// Para dir = (0, 1), o resultado será (-1, 0) * plane_length.
	game->play.cam_x = -game->play.dir_y * plane_length;
	game->play.cam_y = game->play.dir_x * plane_length;
}

void	data_init(t_game *game)
{
	struct timeval tv;

	key_mov_init(game);
	// init_camera_plane(game);
	player_init(game);
	gettimeofday(&tv, NULL);
	game->time.last_frame_time = tv.tv_sec + (tv.tv_usec / 1000000.0);
	game->celling_color = get_color_rgb(30, 30, 150);
	game->floor_color = get_color_rgb(30, 120, 30);
}

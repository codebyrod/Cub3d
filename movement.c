/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 18:18:21 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/01 23:25:50 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	next_move(t_game *game)
{
	double	tmp_x;
	double	tmp_y;
	double	radius_x;
	double	radius_y;

	tmp_x = game->play.play_x + game->play.delta_x;
	radius_x = tmp_x;
	if (game->play.delta_x > 0)
		radius_x = tmp_x + game->play.radius;
	else if (game->play.delta_x < 0)
		radius_x = tmp_x - game->play.radius;

	// PRINT DE TESTE
	printf("X: play_y=%.3f radius_x=%.3f -> map[%d][%d]=%c\n",
	game->play.play_y, radius_x,
	(int)game->play.play_y, (int)radius_x,
	game->map.map[(int)game->play.play_y][(int)radius_x]);

		
	if (game->map.map[(int)game->play.play_y][(int)radius_x] == '0')
		game->play.play_x = tmp_x;
	tmp_y = game->play.play_y + game->play.delta_y;
	radius_y = tmp_y;
	if (game->play.delta_y > 0)
		radius_y = tmp_y + game->play.radius;
	else if (game->play.delta_y < 0)
		radius_y = tmp_y - game->play.radius;
	
	//print de teste
	printf("X: play_y=%.3f radius_x=%.3f -> map[%d][%d]=%c\n",
	game->play.play_y, radius_x,
	(int)game->play.play_y, (int)radius_x,
	game->map.map[(int)game->play.play_y][(int)radius_x]);
	
	if (game->map.map[(int)radius_y][(int)game->play.play_x] == '0')
		game->play.play_y = tmp_y;
	return ;
}

static void	calc_coord_delta(t_game *game)
{
	double	timestamp_current;
	struct timeval tv;
	
	//VERIFICAR SE POSSO USAR ESSA FUNÇÃO
	gettimeofday(&tv, NULL);
	timestamp_current = tv.tv_sec + (tv.tv_usec / 1000000.0);
	game->time.delta_time = timestamp_current - game->time.last_frame_time;
	game->play.delta_x = 0;
	game->play.delta_y = 0;
	if (game->keys.w == BOOL_TRUE || game->keys.a_up == BOOL_TRUE)
		game->play.delta_y -= SPEED_SEC * game->time.delta_time;
	if (game->keys.s == BOOL_TRUE || game->keys.a_bottom == BOOL_TRUE)
		game->play.delta_y += SPEED_SEC * game->time.delta_time;
	if (game->keys.a == BOOL_TRUE || game->keys.a_left == BOOL_TRUE)
		game->play.delta_x -= SPEED_SEC * game->time.delta_time;
	if (game->keys.d == BOOL_TRUE || game->keys.a_right == BOOL_TRUE)
		game->play.delta_x += SPEED_SEC * game->time.delta_time;
	game->play.magnitude = sqrt((game->play.delta_x * game->play.delta_x) 
		+ (game->play.delta_y * game->play.delta_y));
	if (game->play.magnitude > (SPEED_SEC * game->time.delta_time))
	{
		game->play.delta_x = (game->play.delta_x / game->play.magnitude) * (SPEED_SEC * game->time.delta_time); 
		game->play.delta_y = (game->play.delta_y / game->play.magnitude) * (SPEED_SEC * game->time.delta_time);
	}
	game->time.last_frame_time = timestamp_current;
	return ;
}

int	handle_movement(t_game *game)
{	
	calc_coord_delta(game);
	next_move(game);
	game_render(game);
	return (0);
}
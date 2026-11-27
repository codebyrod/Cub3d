/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 18:18:21 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:15:27 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	move_player_dir(t_game *game, double forward, double strafe)
{
	double	dx;
	double	dy;
	double	speed;

	speed = SPEED_SEC * game->time.delta_time;
	dx = (game->play.dir_x * forward - game->play.dir_y * strafe) * speed;
	dy = (game->play.dir_y * forward + game->play.dir_x * strafe) * speed;
	move_with_collision(game, dx, dy);
}

void	next_move(t_game *game)
{
	double	forward;
	double	strafe;

	forward = 0;
	strafe = 0;
	if (game->keys.w)
		forward += 1;
	if (game->keys.s)
		forward -= 1;
	if (game->keys.a)
		strafe -= 1;
	if (game->keys.d)
		strafe += 1;
	move_player_dir(game, forward, strafe);
	return ;
}

void	calc_angle(t_game *game)
{
	double	tmp_x;
	double	tmp_y;
	double	tmp_cam_x;
	double	tmp_cam_y;
	double	ang;

	ang = 0;
	if (game->keys.a_left == BOOL_TRUE)
		ang -= ROT_SPEED_SEC * game->time.delta_time;
	if (game->keys.a_right == BOOL_TRUE)
		ang += ROT_SPEED_SEC * game->time.delta_time;
	ang = ang * (M_PI / 180.0);
	tmp_x = game->play.dir_x * cos(ang) - game->play.dir_y * sin(ang);
	tmp_y = game->play.dir_x * sin(ang) + game->play.dir_y * cos(ang);
	tmp_cam_x = game->play.cam_x * cos(ang) - game->play.cam_y * sin(ang);
	tmp_cam_y = game->play.cam_x * sin(ang) + game->play.cam_y * cos(ang);
	game->play.dir_x = tmp_x;
	game->play.dir_y = tmp_y;
	game->play.cam_x = tmp_cam_x;
	game->play.cam_y = tmp_cam_y;
}

void	calc_coord_delta(t_game *game)
{
	double			timestamp_current;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	timestamp_current = tv.tv_sec + (tv.tv_usec / 1000000.0);
	game->time.delta_time = timestamp_current - game->time.last_frame_time;
	game->play.delta_x = 0;
	game->play.delta_y = 0;
	calc_coord_elta_aux(game);
	game->play.magnitude = sqrt((game->play.delta_x * game->play.delta_x)
			+ (game->play.delta_y * game->play.delta_y));
	if (game->play.magnitude > (SPEED_SEC * game->time.delta_time))
	{
		game->play.delta_x = (game->play.delta_x / game->play.magnitude)
			* (SPEED_SEC * game->time.delta_time);
		game->play.delta_y = (game->play.delta_y / game->play.magnitude)
			* (SPEED_SEC * game->time.delta_time);
	}
	game->time.last_frame_time = timestamp_current;
	return ;
}

int	handle_movement(t_game *game)
{
	calc_coord_delta(game);
	calc_angle(game);
	next_move(game);
	game_render(game);
	return (0);
}

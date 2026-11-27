/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 16:51:30 by rosousa-          #+#    #+#             */
/*   Updated: 2026/11/27 16:57:21 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	calc_coord_elta_aux(t_game *game)
{
	if (game->keys.w == BOOL_TRUE)
		game->play.delta_y -= SPEED_SEC * game->time.delta_time;
	if (game->keys.s == BOOL_TRUE)
		game->play.delta_y += SPEED_SEC * game->time.delta_time;
	if (game->keys.a == BOOL_TRUE)
		game->play.delta_x -= SPEED_SEC * game->time.delta_time;
	if (game->keys.d == BOOL_TRUE)
		game->play.delta_x += SPEED_SEC * game->time.delta_time;
}

void	move_with_collision(t_game *game, double dx, double dy)
{
	double	next_x;
	double	next_y;

	next_x = game->play.play_x + dx;
	if (!is_player_colliding(game, next_x, game->play.play_y))
		game->play.play_x = next_x;
	next_y = game->play.play_y + dy;
	if (!is_player_colliding(game, game->play.play_x, next_y))
		game->play.play_y = next_y;
}

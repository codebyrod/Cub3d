/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:28:31 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:03:53 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	put_player_half(t_game *game, int play_coord_x, int play_coord_y)
{
	int		i;
	int		j;
	int		offset_x;
	int		offset_y;
	double	dot;

	i = 0;
	while (i < PLAYER_SIZE)
	{
		j = 0;
		while (j < PLAYER_SIZE)
		{
			offset_x = i - (PLAYER_SIZE / 2);
			offset_y = j - (PLAYER_SIZE / 2);
			dot = offset_x * game->play.dir_x + offset_y * game->play.dir_y;
			if (dot >= 0)
				my_pixel_put(&game->img, play_coord_x
					+ i, play_coord_y + j, VIOLET);
			else
				my_pixel_put(&game->img, play_coord_x
					+ i, play_coord_y + j, YELLOW);
			j++;
		}
		i++;
	}
}

void	player_render(t_game *game)
{
	int	play_coord_x;
	int	play_coord_y;
	int	pxl_x;
	int	pxl_y;

	pxl_x = game->play.play_x * TILE_SIZE;
	pxl_y = game->play.play_y * TILE_SIZE;
	play_coord_x = pxl_x - (PLAYER_SIZE / 2);
	play_coord_y = pxl_y - (PLAYER_SIZE / 2);
	put_player_half(game, play_coord_x, play_coord_y);
}

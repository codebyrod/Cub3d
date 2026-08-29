/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:35:11 by rodrigo           #+#    #+#             */
/*   Updated: 2026/08/28 22:56:15 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void put_img(t_game *game)
{
	int x;
	int y;

	x = 10;
	y = 10;
	while(x < (HEIGHT / 3))
	{
		y = 10;
		while (y < (WIDTH / 3) )
		{
			mlx_pixel_put(game->connection, game->window, x, y, 0x0000ff);
			y++;
		}
		x++;
	}
	
}

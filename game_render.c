/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:35:11 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/07 22:07:00 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//construir o mapa

//construir o jogador
//calculo da movimentação do jogador

void	test_ray(t_game *game)
{
	t_hit	hit;

	hit = cast_ray(game, game->play.dir_x, game->play.dir_y);
	printf("dist=%.3f side=%d hit=(%.3f, %.3f)\n",
		hit.dist, hit.side, hit.hit_x, hit.hit_y);
}

void	draw_ray_debug(t_game *game, double ray_dx, double ray_dy)
{
	t_hit	hit;
	double	t;
	int		px;
	int		py;

	hit = cast_ray(game, ray_dx, ray_dy);
	t = 0;
	while (t < hit.dist)
	{
		px = (game->play.play_x + ray_dx * t) * TILE_SIZE;
		py = (game->play.play_y + ray_dy * t) * TILE_SIZE;
		my_pixel_put(&game->img, px, py, GREEN);
		t += 0.02;
	}
}


void game_render(t_game *game)
{
	map_render(game);
	// put_tile(game, 400, 0, 0, RED);
	player_render(game);
	draw_ray_debug(game, game->play.dir_x, game->play.dir_y);
	test_ray(game);
	// handle_movement(game);
	mlx_put_image_to_window(game->connection,
		game->window,
		game->img.img_ptr,
		0, 0);
}

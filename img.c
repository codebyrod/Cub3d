/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:35:11 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/01 17:36:55 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//construir o mapa

//construir o jogador
//calculo da movimentação do jogador


void game_render(t_game *game)
{
	map_render(game);
	// put_tile(game, 400, 0, 0, RED);
	player_render(game);
	// handle_movement(game);
	mlx_put_image_to_window(game->connection,
		game->window,
		game->img.img_ptr,
		0, 0);
}

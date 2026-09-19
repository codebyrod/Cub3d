/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:35:11 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/19 14:06:38 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//construir o mapa

//construir o jogador
//calculo da movimentação do jogador





// void	handle_pixel(t_game *game)
// {

// 	//calc_moviment();

// }

void game_render(t_game *game)
{
	// handle_pixel(game);
	map_render(game);
	player_render(game);
	mlx_put_image_to_window(game->connection,
		game->window,
		game->img.img_ptr,
		0, 0);
	
}

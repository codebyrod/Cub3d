/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:28:31 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/01 23:47:15 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"



void	player_render(t_game *game)
{
	//definir posição inicial do jogador
	//deve ser alguma coisa após a parede, o máximo de canto possível
	// origem put_brute
	// pos_init_player(game);

	int	play_coord_x;
	int	play_coord_y;
	int	pxl_x;
	int	pxl_y;

	pxl_x = game->play.play_x * TILE_SIZE;
	pxl_y = game->play.play_y * TILE_SIZE;
	
	play_coord_x = pxl_x - (PLAYER_SIZE / 2);
	play_coord_y = pxl_y - (PLAYER_SIZE / 2);
	
	put_tile(game, PLAYER_SIZE, play_coord_x,
		play_coord_y, VIOLET);
	
}
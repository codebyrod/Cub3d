/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:28:31 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/20 16:58:07 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"



void	player_render(t_game *game)
{
	//definir posição inicial do jogador
	//deve ser alguma coisa após a parede, o máximo de canto possível
	// origem put_brute

	// pos_init_player(game);



	put_tile(game, PLAYER_SIZE, game->play.play_x * TILE_SIZE,
		game->play.play_y * TILE_SIZE, VIOLET);
	
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:28:31 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/19 16:33:09 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	pos_init_player(t_game *game)
{
	int	i;
	int	j;
	i = 0;
	while (i < game->map.map_width)
	{
		j = 0;
		while (j < game->map.map_height)
		{
			if (game->map.map[j][i] == '0')
			{
				game->play.pos_init_X = i;
				game->play.pos_init_Y = j;
				return ;
			}
			j++;
		}	
		i++;
	}
	
}

void	player_render(t_game *game)
{
	//definir posição inicial do jogador
	//deve ser alguma coisa após a parede, o máximo de canto possível
	// origem put_brute

	pos_init_player(game);

	game->play.play_x = game->play.pos_init_X + 0.3;
	game->play.play_y = game->play.pos_init_Y + 0.3;

	put_tile(game, PLAYER_SIZE, game->play.play_x * TILE_SIZE,
		game->play.play_y * TILE_SIZE, VIOLET);
	
}
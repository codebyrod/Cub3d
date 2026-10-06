/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:28:31 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/05 23:11:53 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//FUNÇÃO DE TESTE
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
				my_pixel_put(&game->img, play_coord_x + i, play_coord_y + j, VIOLET);
			else
				my_pixel_put(&game->img, play_coord_x + i, play_coord_y + j, YELLOW);
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
	// pos_init_player(game);

	int	play_coord_x;
	int	play_coord_y;
	int	pxl_x;
	int	pxl_y;

	//Conversãp de mundo para pixel
	pxl_x = game->play.play_x * TILE_SIZE;
	pxl_y = game->play.play_y * TILE_SIZE;
	
	//calculo do canto a partir do centro do jogador
	play_coord_x = pxl_x - (PLAYER_SIZE / 2);
	play_coord_y = pxl_y - (PLAYER_SIZE / 2);
	
	put_player_half(game, play_coord_x, play_coord_y);
	
	// put_tile(game, PLAYER_SIZE, play_coord_x,
	// play_coord_y, VIOLET);
}
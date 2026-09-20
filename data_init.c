/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:55:50 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/20 17:01:29 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	pos_init_player(t_game *game)
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

void	data_init(t_game *game)
{
	pos_init_player(game);
	game->play.play_x = game->play.pos_init_X + 0.3;
	game->play.play_y = game->play.pos_init_Y + 0.3;
	
}
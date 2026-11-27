/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_bridge_game.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 19:08:56 by febranda          #+#    #+#             */
/*   Updated: 2026/11/27 16:28:27 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	config_to_game(t_game *game, t_data_pars *data)
{
	game->cfg = data;
	game->play.play_x = data->player_data.col;
	game->play.play_y = data->player_data.row;
	game->map.map_width = data->map.width;
	game->map.map_height = data->map.height;
	game->play.view_dir = data->player_data.view_direction;
	game->floor_color = data->floor_color;
	game->ceiling_color = data->ceil_color;
	game->path_text.north = data->textures.north;
	game->path_text.south = data->textures.south;
	game->path_text.west = data->textures.west;
	game->path_text.east = data->textures.east;
	game->map.map = data->map.map_layout;
}

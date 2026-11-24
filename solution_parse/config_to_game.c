/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_to_game.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 19:08:56 by febranda          #+#    #+#             */
/*   Updated: 2026/11/24 20:04:56 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include "includes/parser.h"

static size_t	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

static void	free_imperfect_arr(char **arr, size_t count)
{
	if (!arr)
		return ;
	while (count > 0)
	{
		count--;
		free(arr[count]);
	}
	free(arr);
}

static char	**ft_arrdup(char **arr)
{
	size_t	len;
	size_t	i;
	char	**copy;

	i = 0;
	if (!arr)
		return (NULL);
	len = ft_arrlen(arr);
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	while (i < len)
	{
		copy[i] = ft_strdup(arr[i]);
		if (!copy[i])
		{
			free_imperfect_arr(copy, i);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	config_to_game(t_game *game, t_data_pars *data)
{
	game->play.play_x = data->player_data.col;
	game->play.play_y = data->player_data.row;
	game->map.map = data->map.map_layout;
	game->map.map_width = data->map.width;
	game->map.map_height = data->map.height;
	game->play.view_dir = data->player_data.view_direction;
	game->floor_color = data->floor_color;
	game->ceiling_color = data->ceil_color;
	game->textures.north.addr = ft_strdup(data->textures.north);
	game->textures.south.addr = ft_strdup(data->textures.south);
	game->textures.east.addr = ft_strdup(data->textures.east);
	game->textures.west.addr = ft_strdup(data->textures.west);
	game->map.map = ft_arrdup(data->map.map_layout);
}

// erro no arrdup quando libera o mapa original nao libera a copia
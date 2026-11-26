/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 16:28:47 by febranda          #+#    #+#             */
/*   Updated: 2026/11/26 16:28:58 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

static int	is_leaking(t_map_pars *map, int r, int c)
{
	char	**m;

	m = map->map_layout;
	if (r == 0 || c == 0 || r == map->height - 1 || c == map->width - 1)
		return (1);
	return (m[r - 1][c] == ' ' || m[r + 1][c] == ' '
		|| m[r][c - 1] == ' ' || m[r][c + 1] == ' ');
}

static int	check_closed(t_map_pars *map)
{
	int	r;
	int	c;

	r = 0;
	while (map->map_layout[r])
	{
		c = 0;
		while (map->map_layout[r][c])
		{
			if (ft_strchr("0NSEW", map->map_layout[r][c])
				&& is_leaking(map, r, c))
				return (1);
			c++;
		}
		r++;
	}
	return (0);
}

static int	check_basics(t_data_pars *game_config)
{
	if (!game_config->map.map_layout)
	{
		parse_error("Map is missing");
		return (1);
	}
	if (game_config->player_data.count != 1)
	{
		parse_error("Map needs exactly one player start");
		return (1);
	}
	return (0);
}

int	validate_map(t_data_pars *game_config)
{
	if (check_basics(game_config))
		return (1);
	if (setup_map_size(&game_config->map))
		return (1);
	if (check_closed(&game_config->map))
	{
		parse_error("Map is not closed by walls or have space inside");
		return (1);
	}
	return (0);
}

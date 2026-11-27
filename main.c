/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 20:58:53 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:01:45 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

int	main(int ac, char **av)
{
	t_data_pars	config;
	t_game		game;

	if (ac != 2 || !av[1] || !av[1][0])
	{
		parse_error("Usage: ./cub3D <map.cub>");
		return (1);
	}
	ft_bzero(&game, sizeof(game));
	if (parse(av[1], &config))
		return (1);
	config_to_game(&game, &config);
	if (cub_init(&game))
	{
		free_config(&config);
		return (1);
	}
	return (0);
}

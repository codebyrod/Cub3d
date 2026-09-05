/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 20:58:53 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/04 20:24:44 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(void)
{
	t_game game;
	char *map[] = {"11111", "10001", "10101", "11111"};
	int		cub;
	
	game.map.map = map;
	cub = cub_init(&game);
	if (!cub)
	{
		printf("Deu erro na inicialização do cub\n");
	}
	return (0);
}


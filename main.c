/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 20:58:53 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/24 23:16:15 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(void)
{
	// t_game game;
	
	// char	*map[] = {"11111", "10001", "10101", "10012", "11112"};
	// char *map[] = {
	// "111111111111",
	// "100000000001",
	// "100000000001",
	// "100011100001",
	// "100010000001",
	// "100000011100",
	// "100000000001",
	// "111111111111",
	// NULL
	// };

	t_game game;
	


	// char *map[] = {
	// "0j",
	// "m0",
	// // "p0000000000q",
	// // "r000stu0000v",
	// // "x000w000000y",
	// // "z000000ABC00",
	// // "D0000000000E",
	// // "liviaK1MNOPQ1",
	// NULL
	// };
	
	// int		map_width = 2;
	// int		map_height = 2;

	
	int		map_width = 12;
	int		map_height = 8;
	char *map[] = {
	"murilojhoguel",
	"nanijoao000o",
	"p0000000000q",
	"r000stu0000v",
	"x000w000000y",
	"z000000ABC00",
	"D0000000000E",
	"liviaK1MNOPQ1",
	NULL
	};
	int		cub;
	
	game.map.map = map;
	game.map.map_width = map_width;
	game.map.map_height = map_height;
	if ((game.map.map_width * TILE_SIZE) > WIDTH
		|| (game.map.map_height * TILE_SIZE) > HEIGHT)
		printf("Erro, mapa maior que janela\n");
	cub = cub_init(&game);
	if (!cub)
	{
		printf("Deu erro na inicialização do cub\n");
	}
	return (0);
}


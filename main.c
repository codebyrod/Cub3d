/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 20:58:53 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/23 17:14:13 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "cub3d.h"

// int	main_old(void)
// {
// 	// t_game game;
	
// 	// char	*map[] = {"11111", "10001", "10101", "10012", "11112"};
// 	// char *map[] = {
// 	// "111111111111",
// 	// "100000000001",
// 	// "100000000001",
// 	// "100011100001",
// 	// "100010000001",
// 	// "100000011100",
// 	// "100000000001",
// 	// "111111111111",
// 	// NULL
// 	// };

// 	t_game game;
	


// 	// char *map[] = {
// 	// "0j",
// 	// "m0",
// 	// // "p0000000000q",
// 	// // "r000stu0000v",
// 	// // "x000w000000y",
// 	// // "z000000ABC00",
// 	// // "D0000000000E",
// 	// // "liviaK1MNOPQ1",
// 	// NULL
// 	// };
	
// 	// int		map_width = 2;
// 	// int		map_height = 2;

	
// 	int		map_width = 12;
// 	int		map_height = 11;
// 	char *map[] = {
// 	"123456789A12", //
// 	"211111110001",
// 	"300000000001",
// 	"400011100001",
// 	"400011100001",
// 	"400011100001",
// 	"500010000001",
// 	"500010000001",
// 	"400011100001",
// 	"700000000001",
// 	"600000011101",
// 	"600000011101",
// 	"700000000001",
// 	"811110011111",
// 	"911110011111",
// 	"A11110011111",
// 	"111111110111",
// 	NULL
// 	};
	
// 	game.map.map = map;
// 	game.map.map_width = map_width;
// 	game.map.map_height = map_height;
// 	if ((game.map.map_width * TILE_SIZE) > WIDTH
// 		|| (game.map.map_height * TILE_SIZE) > HEIGHT)
// 	{
// 		ft_putstr_fd("Error, map larger than window\n", 2);
// 		return (1);		
// 	}
	
// 	// printf(" MAP - Main::: map_width=%d map_height=%d\n",
// 	// game.map.map_width, game.map.map_height);
// 	if (!cub_init(&game))
// 	{
// 		ft_putstr_fd("Error\n", 2);
// 		return (1);
// 	}
// 	return (0);
// }


//New Main
#include "includes/cub3d.h"

static void	debug_print_config(t_data_pars *c);


int	main(int ac, char **av)
{
	t_data_pars	config;
	t_game	game;

	if (ac != 2 || !av[1] || !av[1][0])
	{
		parse_error("Usage: ./cub3D <map.cub>");
		return (1);
	}
                   // TEMPORÁRIO
	ft_bzero(&game, sizeof(game));
	if (parse(av[1], &config))
		return (1);
	
	// debug_print_config(&config);   // TEMPORÁRIO
	// free_config(&config);          // TEMPORÁRIO: encerra aqui para só testar o parser
	// return (0);
	
	config_to_game(&config, &game);
	// game.cfg = &config;
	if (cub_init(&game))
	{
		free_config(&config);
		return (1);
	}
	return (0);
}

//TEMPORÁRIO
static void	debug_print_config(t_data_pars *c)
{
	int	i;

	printf("---- DEBUG PARSER ----\n");
	printf("map.width    = %d\n", c->map.width);
	printf("map.height   = %d\n", c->map.height);
	printf("north        = [%s]\n", c->textures.north);
	printf("south        = [%s]\n", c->textures.south);
	printf("west         = [%s]\n", c->textures.west);
	printf("east         = [%s]\n", c->textures.east);
	printf("floor_color  = %d (0x%06X)\n", c->floor_color, c->floor_color);
	printf("ceil_color   = %d (0x%06X)\n", c->ceil_color, c->ceil_color);
	printf("player row   = %d\n", c->player_data.row);
	printf("player col   = %d\n", c->player_data.col);
	printf("player dir   = %c\n", c->player_data.view_direction);
	printf("---- LAYOUT ----\n");
	i = 0;
	while (c->map.map_layout && c->map.map_layout[i])
	{
		printf("%2d [%s]\n", i, c->map.map_layout[i]);
		i++;
	}
	printf("----------------------\n");
}
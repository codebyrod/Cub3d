/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 20:58:53 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/25 03:29:34 by rosousa-         ###   ########.fr       */
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

// static void	debug_print_config(t_data_pars *c);
static void	debug_print_bridge(t_game *game, t_data_pars *data);
static void	debug_print_map_compare(char **src, char **dst);

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
	printf("apos parse: %d '%c'\n", config.player_data.view_direction, config.player_data.view_direction);
	
	config_to_game(&game, &config);
	
	printf("view_direction: %d '%c'\n", config.player_data.view_direction,
		config.player_data.view_direction);
	
	debug_print_bridge(&game, &config);
	
	if (cub_init(&game))
	{
		free_config(&config);
		return (1);
	}
	return (0);
}

//TEMPORÁRIO
// static void	debug_print_config(t_game *c, t_data_pars *c)
// {
// 	int	i;

// 	printf("---- DEBUG PARSER ----\n");
// 	printf("map.width    = %d\n", c->map.width);
// 	printf("map.height   = %d\n", c->map.height);
// 	printf("north        = [%s]\n", c->textures.north);
// 	printf("south        = [%s]\n", c->textures.south);
// 	printf("west         = [%s]\n", c->textures.west);
// 	printf("east         = [%s]\n", c->textures.east);
// 	printf("floor_color  = %d (0x%06X)\n", c->floor_color, c->floor_color);
// 	printf("ceil_color   = %d (0x%06X)\n", c->ceil_color, c->ceil_color);
// 	printf("player row   = %d\n", c->player_data.row);
// 	printf("player col   = %d\n", c->player_data.col);
// 	printf("player dir   = %c\n", c->player_data.view_direction);
// 	printf("---- LAYOUT ----\n");
// 	i = 0;
// 	while (c->map.map_layout && c->map.map_layout[i])
// 	{
// 		printf("%2d [%s]\n", i, c->map.map_layout[i]);
// 		i++;
// 	}
// 	printf("----------------------\n");
// }

static void	debug_print_map_compare(char **src, char **dst)
{
	int	i;

	i = 0;
	while ((src && src[i]) || (dst && dst[i]))
	{
		printf("  linha %2d | data: [%s]\n", i, (src && src[i]) ? src[i] : "NULL");
		printf("           | game: [%s]\n", (dst && dst[i]) ? dst[i] : "NULL");
		i++;
	}
}

static void	debug_print_bridge(t_game *game, t_data_pars *data)
{
	printf("========== DEBUG BRIDGE (data -> game) ==========\n");
	printf("[player_data.col -> play.play_x]\n");
	printf("  data: %d\n", data->player_data.col);
	printf("  game: %f\n", game->play.play_x);
	printf("[player_data.row -> play.play_y]\n");
	printf("  data: %d\n", data->player_data.row);
	printf("  game: %f\n", game->play.play_y);
	printf("[map.width -> map.map_width]\n");
	printf("  data: %d\n", data->map.width);
	printf("  game: %d\n", game->map.map_width);
	printf("[map.height -> map.map_height]\n");
	printf("  data: %d\n", data->map.height);
	printf("  game: %d\n", game->map.map_height);
	printf("[player_data.view_direction -> play.view_dir]\n");
	printf("  data: %c\n", data->player_data.view_direction);
	printf("  game: %c\n", game->play.view_dir);
	printf("[floor_color -> floor_color]\n");
	printf("  data: %d (0x%06X)\n", data->floor_color, data->floor_color);
	printf("  game: %d (0x%06X)\n", game->floor_color, game->floor_color);
	printf("[ceil_color -> ceiling_color]\n");
	printf("  data: %d (0x%06X)\n", data->ceil_color, data->ceil_color);
	printf("  game: %d (0x%06X)\n", game->ceiling_color, game->ceiling_color);
	// printf("[textures.north -> textures.north.addr]\n");
	// printf("  data: [%s]\n", data->textures.north);
	// printf("  game: [%s]\n", game->textures.north.addr);
	// printf("[textures.south -> textures.south.addr]\n");
	// printf("  data: [%s]\n", data->textures.south);
	// printf("  game: [%s]\n", game->textures.south.addr);
	// printf("[textures.east -> textures.east.addr]\n");
	// printf("  data: [%s]\n", data->textures.east);
	// printf("  game: [%s]\n", game->textures.east.addr);
	// printf("[textures.west -> textures.west.addr]\n");
	// printf("  data: [%s]\n", data->textures.west);
	// printf("  game: [%s]\n", game->textures.west.addr);
	printf("[map.map_layout -> map.map]\n");
	debug_print_map_compare(data->map.map_layout, game->map.map);
	printf("=================================================\n");
}
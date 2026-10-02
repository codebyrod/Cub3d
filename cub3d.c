/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 02:39:51 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/01 17:35:57 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//resolver isso de alguma forma, fiz de um jeito legal no fractol, olhar depois
int	cub_connection(t_game *game)
{
	game->connection = mlx_init();
	if (!game->connection)
	{
		ft_putstr_fd("Error allocating the Minilibx connection [mlx_init]", 2);
		return (MALLOC_ERROR);
	}
	game->window = mlx_new_window(game->connection, WIDTH, HEIGHT, "Cub3D");
	// game->window = mlx_new_window(game->connection, HEIGHT, WIDTH, "Cub3D");
	if (!game->window)
	{
		err_init_cub(game, "window");
		return (MALLOC_ERROR);
	}
	game->img.img_ptr = mlx_new_image(game->connection, WIDTH, HEIGHT);
	if (!game->img.img_ptr)
	{
		err_init_cub(game, "img_ptr");
		return (MALLOC_ERROR);
	}
	game->img.img_pixels_ptr = mlx_get_data_addr(game->img.img_ptr, 
		&game->img.bits_per_pixel, &game->img.size_len, &game->img.endian);
	if (!game->img.img_pixels_ptr)
	{
		err_init_cub(game, "img_pixel");
		return (MALLOC_ERROR);
	}
	return (0);
}

void	end_connection(t_game *game)
{
	mlx_destroy_window(game->connection, game->window);
	mlx_destroy_display(game->connection);
	free(game->connection);
}
// int	close_handler(t_game *game)
// {
// 	mlx_destroy_image(game->connection, game->img.img_ptr);
// 	mlx_destroy_window(game->connection, game->window);
// 	mlx_destroy_display(game->connection);
// 	free(game->connection);
// 	exit (EXIT_SUCCESS);
// }

int cub_init(t_game *game)
{
	// printf(" MAP - 1::: map_width=%d map_height=%d\n",
	// game->map.map_width, game->map.map_height);
	printf("CHECK 1: width=%d\n", game->map.map_width);
	cub_connection(game);
	printf("CHECK 2: width=%d\n", game->map.map_width);
	data_init(game);
	// printf(" MAP - 2::: map_width=%d map_height=%d\n",
	// game->map.map_width, game->map.map_height);
	printf("CHECK 3: width=%d\n", game->map.map_width);
	events_init(game);
	printf("CHECK 4: width=%d\n", game->map.map_width);
	game_render(game);
	printf("CHECK 5: width=%d\n", game->map.map_width);
	// put_img(game);
	//limpar depois de fechar a janela
	mlx_loop(game->connection); //mantém a janela aberta
	// end_connection(game); se deicar essa linha depois do loop ela fecha com segfault
	return (0);
}

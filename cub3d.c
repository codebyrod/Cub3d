/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 02:39:51 by rodrigo           #+#    #+#             */
/*   Updated: 2026/08/28 22:02:06 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// void	events_init(t_game *game)
// {
// 	(void)game;
// 	mlx_hook(game->window, KeyPress, KeyPressMask,
// 		handle_movement,  game);
// 	mlx_hook(fractal->window, DestroyNotify,
// 		StructureNotifyMask, close_handler, fractal);
// 	printf("events\n");
// }

//resolver isso de alguma forma
int	cub_connection(t_game *game)
{
	game->connection = mlx_init();
	if (!game->connection)
	{
		ft_putstr_fd("Error allocating the Minilibx connection [mlx_init]", 2);
		return (MALLOC_ERROR);
	}
	game->window = mlx_new_window(game->connection, HEIGHT, WIDTH, "Cub3D");
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

int cub_init(t_game *game)
{
	cub_connection(game);
	mlx_loop(game->connection); //mantém a janela aberta
	// events_init(game);

	//limpar depois de fechar a janela
	end_connection(game);
	return (0);
}

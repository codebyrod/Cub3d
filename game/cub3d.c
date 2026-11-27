/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 02:39:51 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 16:59:34 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	cub_connection_aux(t_game *game)
{
	game->connection = mlx_init();
	if (!game->connection)
	{
		ft_putstr_fd("Error allocating the Minilibx connection [mlx_init]", 2);
		return (MALLOC_ERROR);
	}
	game->window = mlx_new_window(game->connection, WIDTH, HEIGHT, "Cub3D");
	if (!game->window)
	{
		err_init_cub(game, "window");
		return (MALLOC_ERROR);
	}
	return (SUCESS);
}

int	cub_connection(t_game *game)
{
	if (cub_connection_aux(game))
		return (MALLOC_ERROR);
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
	return (SUCESS);
}

int	cub_init(t_game *game)
{
	if (cub_connection(game))
		return (MALLOC_ERROR);
	if (textures_init(game))
	{
		end_connection(game);
		printf("Could not load a texture\n");
		return (TEXTURE_ERROR);
	}
	data_init(game);
	events_init(game);
	game_render(game);
	mlx_loop(game->connection);
	return (0);
}

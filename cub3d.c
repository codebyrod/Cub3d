/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 02:39:51 by rodrigo           #+#    #+#             */
/*   Updated: 2026/08/27 21:02:01 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	events_init(t_game game)
{
	(void)game;

	printf("events\n");
}

int cub_init(t_game game)
{

	game.connection = mlx_init();
	if (!game.connection)
		return (MALLOC_ERROR);
	game.window = mlx_new_window(game.connection, HEIGHT, WIDTH, "Cub3D");
	if (!game.window)
	{
		//destruir conexão
		mlx_destroy_display(game.connection);
		free(game.connection);
		return (MALLOC_ERROR);
	}

	game.img.img_ptr = mlx_new_image(game.connection, WIDTH, HEIGHT);
	if (!game.img.img_ptr)
	{
		//destruir window
		//destruir connection
		//limpar ptr de connection
		return (MALLOC_ERROR);
	}

	game.img.img_pixels_ptr = mlx_get_data_addr(
				game.img.img_ptr, &game.img.bits_per_pixel,
				&game.img.size_len, &game.img.endian);
	if (!game.img.img_pixels_ptr)
	{
		//limpar ptr de img_ptr
		//destruir a window
		//destruir a conexão
		//limpar o ponteiro da conexão
		return (MALLOC_ERROR);
	}
	
	mlx_loop(game.connection); //mantém a janela aberta
	
	//limpar depois de fechar a janela
	mlx_destroy_window(game.connection, game.window);
	mlx_destroy_display(game.connection);
	free(game.connection);


	events_init(game);
	return (0);
}
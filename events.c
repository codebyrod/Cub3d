/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 21:52:34 by rodrigo           #+#    #+#             */
/*   Updated: 2026/08/29 00:57:34 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_movement(t_game *game)
{
	(void)game;
}

void	close_handler(t_game *game)
{
	mlx_destroy_image(game->connection, game->img.img_ptr);
	mlx_destroy_window(game->connection, game->window);
	mlx_destroy_display(game->connection);
	free(game->connection);
}

void	events_init(t_game *game)
{
	(void)game;
	mlx_hook(game->window, KeyPress, KeyPressMask,
		handle_movement,  game);
	mlx_hook(game->window, DestroyNotify,
		StructureNotifyMask, close_handler, game);
	printf("events\n");
}
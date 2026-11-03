/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 20:32:38 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/02 20:59:20 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clear_connection(t_game *game)
{
	if (game->connection)
	{
		mlx_destroy_display(game->connection);
		free(game->connection);
		game->connection = NULL;
	}
}

void	clear_window(t_game *game)
{
	if (game->connection && game->window)
	{
		// mlx_clear_window(game->connection, game->window);
		mlx_destroy_window(game->connection, game->window);
		game->window = NULL;		
	}
}
void	clear_img_ptr(t_game *game)
{
	if (game->connection && game->img.img_ptr)
	{
		mlx_destroy_image(game->connection, game->img.img_ptr);
		game->img.img_ptr = NULL;
		game->img.img_pixels_ptr = NULL;
	}
}
void	end_connection(t_game *game)
{
	clear_img_ptr(game);
	clear_window(game);
	clear_connection(game);
}
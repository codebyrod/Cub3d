/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 00:56:38 by rosousa-          #+#    #+#             */
/*   Updated: 2026/11/25 03:41:06 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	clear_one_texture(t_game *game, t_img *text)
{
	if (game->connection && text->img_ptr)
	{
		mlx_destroy_image(game->connection, text->img_ptr);
		text->img_ptr = NULL;
		text->img_pixels_ptr = NULL;
	}
}

void	clear_textures(t_game *game)
{
	clear_one_texture(game, &game->textures.north);
	clear_one_texture(game, &game->textures.south);
	clear_one_texture(game, &game->textures.east);
	clear_one_texture(game, &game->textures.west);
}

int	close_handler(t_game *game)
{
	clear_textures(game);
	clear_img_ptr(game);
	clear_window(game);
	clear_connection(game);
	free_config(game);
	exit (EXIT_SUCCESS);
}
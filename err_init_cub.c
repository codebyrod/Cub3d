/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_init_cub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 21:04:37 by rodrigo           #+#    #+#             */
/*   Updated: 2026/08/27 23:02:10 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void err_init_cub(t_game game, char *str)
{
	if(ft_strscmp(str, "window"))
	{
		clear_window(game);
		ft_putstr_fd("Error allocating the Minilibx window [mlx_new_window]", 2);
	}
	if(ft_strscmp(str, "img_ptr"))
	{
		clear_img_ptr(game);
		ft_putstr_fd("Error allocating image address Minilibx [mlx_new_image]", 2);		
	}
	if(ft_strscmp(str, "img_pixel"))
	{
		clear_img_pixel(game);
		ft_putstr_fd("Error allocating framebuffer for MiniLibX image [mlx_get_data_addr]", 2);
	}
}

void	clear_connection(t_game game)
{
	mlx_destroy_display(game.connection);
	free(game.connection);
}

void	clear_window(t_game game)
{
	mlx_clear_window(game.connection, game.window);
	clear_connection(game);
}
void	clear_img_ptr(t_game game)
{
	mlx_destroy_image(game.connection, game.img.img_ptr);
	clear_window(game);
}
void	clear_img_pixel(t_game game)
{
	free(game.img.img_pixels_ptr);
	clear_img_ptr(game);
}
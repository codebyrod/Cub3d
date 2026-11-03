/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_init_cub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 21:04:37 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/02 21:29:04 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void err_init_cub(t_game *game, char *str)
{
	if(ft_strscmp(str, "window"))
	{
		clear_connection(game);
		ft_putstr_fd("Error allocating the Minilibx window [mlx_new_window]\n", 2);
	}
	if(ft_strscmp(str, "img_ptr"))
	{
		clear_window(game);
		clear_connection(game);
		ft_putstr_fd("Error allocating image address Minilibx [mlx_new_image]\n", 2);		
	}
	if(ft_strscmp(str, "img_pixel"))
	{
		clear_img_ptr(game);
		clear_window(game);
		clear_connection(game);
		ft_putstr_fd("Error allocating framebuffer for MiniLibX image [mlx_get_data_addr]\n", 2);
	}
}


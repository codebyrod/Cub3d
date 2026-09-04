/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:10:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/04 18:57:06 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"



void	my_pixel_put(t_img *img, int x, int y, int color)
{
	int				displacement;
	int				conv_bit_to_byte;
	char			*addr_to_drawing;
	unsigned int	*int_addr_drawing;

	conv_bit_to_byte = (img->bits_per_pixel / 8);
	displacement = (img->size_len * y) + (x * conv_bit_to_byte);
	addr_to_drawing = displacement + img->img_pixels_ptr;
	*(unsigned int *)addr_to_drawing = color;
	int_addr_drawing = (unsigned int *)(addr_to_drawing);
	*int_addr_drawing = (unsigned int)color;
}

void	put_tile(t_game *game, int color)
{
	int	temp_x;
	int	temp_y;

	temp_x = game->pxl.pxl_x;
	temp_y = game->pxl.pxl_y;
	
	while(temp_x > (TILE_SIZE + game->pxl.pxl_x))
	{
		temp_y = 0;
		while(temp_y > (TILE_SIZE + game->pxl.pxl_y))
		{
			//como pintar um tile de cada cor?
			//impares de uma cor e pares de uma outra cor
			my_pixel_put(&game->img, temp_x, temp_y, color);
			temp_y++;
		}
		temp_x++;
	}
}



void	map_render(t_game *game)
{
	int x;
	int y;

	x = 0;
	while(x < WIDTH)
	{
		y = 0;
		while (y < WIDTH)
		{
			if(y > (WIDTH / 2))
				my_pixel_put(&game->img, x, y, BLUE);
				
			else
				my_pixel_put(&game->img, x, y, RED);
			
			y++;
		}
		x++;
	}
}
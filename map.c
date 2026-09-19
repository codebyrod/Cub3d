/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:10:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/19 14:16:14 by rodrigo          ###   ########.fr       */
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

void	put_tile(t_game *game, int SIZE, int coord_x, int coord_y, int color)
{
	int	i;
	int	j;
	int	temp_x;
	int	temp_y;
	
	i = 0;
	temp_x = coord_x;
	while(i < SIZE)
	{
		j = 0;
		temp_y = coord_y;
		while(j < SIZE)
		{
			my_pixel_put(&game->img, temp_x, temp_y, color);
			j++;
			temp_y++;
		}
		i++;
		temp_x++;
	}
}

void	map_render(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while(i < game->map.map_width)
	{
		j = 0;
		while (j < game->map.map_height)
		{
			if(game->map.map[j][i] == '0')
				put_tile(game, TILE_SIZE, i*TILE_SIZE, j*TILE_SIZE, BLUE);
			else if (game->map.map[j][i] > 64)
				put_tile(game, TILE_SIZE, i*TILE_SIZE, j*TILE_SIZE, YELLOW);
			else
				put_tile(game, TILE_SIZE, i*TILE_SIZE, j*TILE_SIZE, RED);
			j++;
		}
		i++;
	}
}

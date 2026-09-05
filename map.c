/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:10:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/04 23:55:33 by rodrigo          ###   ########.fr       */
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

// void	put_tile(t_game *game, int coord_x, int coord_y, int color)
// {
// 	int	temp_x;
// 	int	temp_y;

// 	// temp_x = game->pxl.pxl_x;
// 	// temp_y = game->pxl.pxl_y;
	
	
// 	temp_x = coord_x;
	
// 	while(temp_x < (TILE_SIZE + coord_x))
// 	{
// 		temp_y = coord_y;
// 		while(temp_y < (TILE_SIZE + coord_y))
// 		{
// 			//como pintar um tile de cada cor?
// 			//impares de uma cor e pares de uma outra cor
// 			my_pixel_put(&game->img, coord_x, coord_y, color);
// 			coord_y++;
// 		}
// 		temp_x++;
// 	}
// 	// game->pxl.pxl_x += TILE_SIZE;
// 	// game->pxl.pxl_y += TILE_SIZE;
// }

void	put_tile(t_game *game, int coord_x, int coord_y, int color)
{
	int	i;
	int	j;
	int	temp_x;
	int	temp_y;
	
	i = 0;
	temp_x = coord_x;
	while(i < TILE_SIZE)
	{
		j = 0;
		temp_y = coord_y;
		while(j < TILE_SIZE)
		{
			//como pintar um tile de cada cor?
			//impares de uma cor e pares de uma outra cor
			my_pixel_put(&game->img, temp_x, temp_y, color);
			j++;
			temp_y++;
		}
		i++;
		temp_x++;
	}
	// game->pxl.pxl_x += TILE_SIZE;
	// game->pxl.pxl_y += TILE_SIZE;
}

void	map_render(t_game *game)
{
	// int	i;
	// int	j;

	// i = 0;
	// while(game->map.map[i])
	// {
	// 	j = 0;
	// 	while (game->map.map[i][j])
	// 	{
	// 		if(game->map.map[i][j] == '1')
	// 			put_tile(game, BLUE);
	// 		else
	// 			put_tile(game, RED);		
	// 		j++;
	// 	}
	// 	i++;
	// }
	put_tile(game, 50, 100, BLUE);
}


// void	map_render(t_game *game)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	while(i < game->map.map_width)
// 	{
// 		j = 0;
// 		while (j < game->map.map_height)
// 		{
// 			if(j > (game->map.map_height / 2))
// 				put_tile(game, BLUE);	
// 			else
// 				put_tile(game, RED);			
// 			j++;
// 		}
// 		i++;
// 	}
// }


// void	map_render(t_game *game)
// {
// 	int cel_x;
// 	int cel_y;
// 	int	x;
// 	int y;

// 	x = 0;
// 	y = 0;
// 	cel_x = WIDTH / TILE_SIZE;
// 	cel_y = HEIGHT / TILE_SIZE;
// 	while(x < cel_x)
// 	{
// 		y = 0;
// 		while (y < cel_y)
// 		{
// 			if(y > (WIDTH / 2))
// 				put_tile(game, BLUE);
				
// 			else
// 				put_tile(game, RED);
			
// 			y++;
// 		}
// 		x++;
// 	}
// }


// void	map_render(t_game *game)
// {
// 	int x;
// 	int y;

// 	x = 0;
// 	while(x < WIDTH)
// 	{
// 		y = 0;
// 		while (y < WIDTH)
// 		{
// 			if(y > (WIDTH / 2))
// 				put_tile(game, BLUE);
				
// 			else
// 				put_tile(game, RED);
			
// 			y++;
// 		}
// 		x++;
// 	}
// }
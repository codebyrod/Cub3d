/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:10:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:26:16 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	int		displacement;
	int		conv_bit_to_byte;
	char	*addr_to_drawing;

	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
		return ;
	conv_bit_to_byte = (img->bits_per_pixel / 8);
	displacement = (img->size_len * y) + (x * conv_bit_to_byte);
	addr_to_drawing = displacement + img->img_pixels_ptr;
	*(unsigned int *)addr_to_drawing = color;
}

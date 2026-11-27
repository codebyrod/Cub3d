/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 20:17:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:09:02 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	load_texture(t_game *game, t_img *texture, char *path)
{
	texture->img_ptr = mlx_xpm_file_to_image(
			game->connection, path, &texture->width, &texture->height);
	if (!texture->img_ptr)
		return (1);
	texture->img_pixels_ptr = mlx_get_data_addr(
			texture->img_ptr, &texture->bits_per_pixel,
			&texture->size_len, &texture->endian);
	if (!texture->img_pixels_ptr)
		return (1);
	return (0);
}

t_img	*get_wall_texture(t_game *game, t_side	side)
{
	if (side == SIDE_N)
		return (&game->textures.north);
	if (side == SIDE_S)
		return (&game->textures.south);
	if (side == SIDE_E)
		return (&game->textures.east);
	if (side == SIDE_W)
		return (&game->textures.west);
	return (NULL);
}

int	get_texture_x(t_hit hit, t_img *textures)
{
	double	wall_x;
	int		tex_x;

	if (hit.side == SIDE_W || hit.side == SIDE_E)
		wall_x = hit.hit_y;
	else
		wall_x = hit.hit_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * textures->width);
	if (hit.side == SIDE_W || hit.side == SIDE_S)
		tex_x = textures->width - tex_x - 1;
	return (tex_x);
}

//vamos pegar a cor da textura
int	get_texture_color(t_img *texture, int x, int y)
{
	int				displacement;
	int				bytes_per_pixel;
	unsigned int	color;

	bytes_per_pixel = texture->bits_per_pixel / 8;
	displacement = (texture->size_len * y) + (x * bytes_per_pixel);
	color = *(unsigned int *)(texture->img_pixels_ptr + displacement);
	return ((int)color);
}

int	textures_init(t_game *game)
{
	if (load_texture(game, &game->textures.north, game->path_text.north))
		return (1);
	if (load_texture(game, &game->textures.south, game->path_text.south))
		return (1);
	if (load_texture(game, &game->textures.west, game->path_text.west))
		return (1);
	if (load_texture(game, &game->textures.east, game->path_text.east))
		return (1);
	return (0);
}

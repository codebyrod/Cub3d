/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 20:17:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/01 23:58:25 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


//essa é a função que criamos para conectar a textura com a imagem
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

//já temos uma função que pega o lado da parede
//agora precisamos de uma função que seleciona a textura de acordo com o lado da parede

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

//agora que já temos o lada da textura, o lado da parede  e a imagem da parede
//agora vamos a calcular pra extrair

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

//agora que transformamos o png em xml precisamos inicializar pra chamar a função de load
//depois no parser a gente tirar isso aqui
// é só algo mockado temporário
int	textures_init(t_game *game)
{
	if (load_texture(game, &game->textures.north, "./textures/north.xpm"))
		return (1);
	if (load_texture(game, &game->textures.south, "./textures/south.xpm"))
		return (1);
	if (load_texture(game, &game->textures.west, "./textures/west.xpm"))
		return (1);
	if (load_texture(game, &game->textures.east, "./textures/east.xpm"))
		return (1);
	return (0);
}
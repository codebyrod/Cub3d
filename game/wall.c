/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:42:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:11:10 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

double	calc_perp_dist(t_game *game, t_hit hit)
{
	double	delta_x;
	double	delta_y;
	double	distance;

	delta_x = hit.hit_x - game->play.play_x;
	delta_y = hit.hit_y - game->play.play_y;
	distance = delta_x * game->play.dir_x + delta_y * game->play.dir_y;
	if (distance < 0.001)
		distance = 0.001;
	return (distance);
}

int	calc_wall_height(double perp_dist)
{
	double	focal_length;
	double	wall_height;

	focal_length = (WIDTH / 2.0) / tan(FOV_DEG * M_PI / 360.0);
	wall_height = focal_length / perp_dist;
	return ((int)wall_height);
}

void	draw_wall_column(t_game *game, int x, int wall_h, t_hit hit)
{
	t_wall	wall;
	t_img	*texture;

	texture = get_wall_texture(game, hit.side);
	if (!texture)
		return ;
	wall.wall_top = (HEIGHT - wall_h) / 2;
	wall.draw_start = wall.wall_top;
	wall.draw_end = wall.wall_top + wall_h;
	if (wall.draw_start < 0)
		wall.draw_start = 0;
	if (wall.draw_end > HEIGHT)
		wall.draw_end = HEIGHT;
	wall.tex_x = get_texture_x(hit, texture);
	wall.y = wall.draw_start;
	while (wall.y < wall.draw_end)
	{
		wall.tex_y = ((wall.y - wall.wall_top) * texture->height) / wall_h;
		wall.color = get_texture_color(texture, wall.tex_x, wall.tex_y);
		my_pixel_put(&game->img, x, wall.y, wall.color);
		wall.y++;
	}
	return ;
}

void	render_wall_column(t_game *game, int x)
{
	t_hit	hit;
	double	ray_dx;
	double	ray_dy;
	double	perp_dist;
	int		wall_height;

	get_camera_ray(game, x, &ray_dx, &ray_dy);
	hit = cast_ray(game, ray_dx, ray_dy);
	if (hit.side == SIDE_NONE)
		return ;
	perp_dist = calc_perp_dist(game, hit);
	wall_height = calc_wall_height(perp_dist);
	draw_wall_column(game, x, wall_height, hit);
	return ;
}

void	render_walls(t_game *game)
{
	int	x;

	x = 0;
	while (x < WIDTH)
	{
		render_wall_column(game, x);
		x++;
	}
	return ;
}

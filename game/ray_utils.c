/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 15:55:32 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:07:23 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_ray_delta(t_ray *ray)
{
	if (ray->ray_dx == 0.0)
		ray->delta_dist_x = INFINITY;
	else
		ray->delta_dist_x = fabs(1.0 / ray->ray_dx);
	if (ray->ray_dy == 0.0)
		ray->delta_dist_y = INFINITY;
	else
		ray->delta_dist_y = fabs(1.0 / ray->ray_dy);
}

void	init_ray_step_x(t_game *game, t_ray *ray)
{
	if (ray->ray_dx < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->play.play_x - ray->map_x) * ray->delta_dist_x;
	}
	else if (ray->ray_dx > 0)
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0
				- game->play.play_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 0;
		ray->side_dist_x = INFINITY;
	}
}

void	init_ray_step_y(t_game *game, t_ray *ray)
{
	if (ray->ray_dy < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->play.play_y - ray->map_y) * ray->delta_dist_y;
	}
	else if (ray->ray_dy > 0)
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0
				- game->play.play_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 0;
		ray->side_dist_y = INFINITY;
	}
}

int	is_wall_at(t_game *game, double x, double y)
{
	int	cx;
	int	cy;

	if (x < 0 || y < 0)
		return (1);
	cx = (int)x;
	cy = (int)y;
	if (cx >= game->map.map_width || cy >= game->map.map_height)
		return (1);
	if (game->map.map[cy][cx] != '0')
		return (1);
	return (0);
}

int	is_player_colliding(t_game *game, double x, double y)
{
	double	radius;

	radius = game->play.radius + 0.02;
	if (is_wall_at(game, x - radius, y))
		return (1);
	if (is_wall_at(game, x + radius, y))
		return (1);
	if (is_wall_at(game, x, y - radius))
		return (1);
	if (is_wall_at(game, x, y + radius))
		return (1);
	return (0);
}

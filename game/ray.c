/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 21:57:18 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:08:14 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	advance_ray(t_ray *ray)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->dist = ray->side_dist_x;
		ray->side_dist_x += ray->delta_dist_x;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->dist = ray->side_dist_y;
		ray->side_dist_y += ray->delta_dist_y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

t_hit	trace_ray(t_game *game, t_ray *ray, t_hit hit)
{
	while (ray->dist < RAY_MAX_DIST)
	{
		advance_ray(ray);
		if (ray->dist >= RAY_MAX_DIST)
			return (hit);
		if (is_wall_at(game, ray->map_x, ray->map_y))
		{
			set_hit(game, ray, &hit);
			return (hit);
		}
	}
	return (hit);
}

void	init_ray(t_game *game, t_ray *ray, double ray_dx, double ray_dy)
{
	ray->ray_dx = ray_dx;
	ray->ray_dy = ray_dy;
	ray->map_x = (int)game->play.play_x;
	ray->map_y = (int)game->play.play_y;
	ray->dist = 0.0;
}

t_hit	cast_ray(t_game *game, double ray_dx, double ray_dy)
{
	t_hit	hit;
	t_ray	ray;

	hit.dist = RAY_MAX_DIST;
	hit.side = SIDE_NONE;
	hit.hit_x = game->play.play_x;
	hit.hit_y = game->play.play_y;
	if (ray_dx == 0.0 && ray_dy == 0.0)
		return (hit);
	init_ray(game, &ray, ray_dx, ray_dy);
	init_ray_delta(&ray);
	init_ray_step_x(game, &ray);
	init_ray_step_y(game, &ray);
	hit = trace_ray(game, &ray, hit);
	return (hit);
}

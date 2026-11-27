/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 20:12:26 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 17:07:05 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	get_color_rgb(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

t_side	get_side(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->ray_dx > 0)
			return (SIDE_W);
		return (SIDE_E);
	}
	if (ray->ray_dy > 0)
		return (SIDE_N);
	return (SIDE_S);
}

void	set_hit(t_game *game, t_ray *ray, t_hit *hit)
{
	hit->dist = ray->dist;
	hit->side = get_side(ray);
	hit->hit_x = game->play.play_x + ray->ray_dx * ray->dist;
	hit->hit_y = game->play.play_y + ray->ray_dy * ray->dist;
}

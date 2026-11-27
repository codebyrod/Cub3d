/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 21:39:36 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/27 16:25:01 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	get_camera_ray(t_game *game, int sc_x, double *ray_dx, double *ray_dy)
{
	double	camera_x;

	camera_x = (2.0 * sc_x / (double)WIDTH) - 1.0;
	*ray_dx = game->play.dir_x + game->play.cam_x * camera_x;
	*ray_dy = game->play.dir_y + game->play.cam_y * camera_x;
}

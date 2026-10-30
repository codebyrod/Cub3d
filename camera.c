/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 21:39:36 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/29 21:43:13 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//   Calcula a direção de um raio para uma coluna da tela.
//   screen_x representa a posição horizontal do pixel.
//   ray_dx e ray_dy recebem a direção calculada.
void	get_camera_ray(t_game *game, int screen_x, double *ray_dx, double *ray_dy)
{
	double	camera_x;

	// Converte a posição horizontal da tela para o espaço da câmera.
	// O lado esquerdo representa aproximadamente -1.
	// O lado direito representa valores próximos de +1.
	camera_x = (2.0 * screen_x / (double)WIDTH) - 1.0;

	// Calcula a componente X da direção do raio.
	// Combina a direção do jogador com o plano da câmera.
	*ray_dx = game->play.dir_x + game->play.cam_x * camera_x;

	// Calcula a componente Y da direção do raio.
	*ray_dy = game->play.dir_y + game->play.cam_y * camera_x;
}

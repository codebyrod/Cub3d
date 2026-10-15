/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 21:57:18 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/14 23:30:54 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_wall_at(t_game *game, double x, double y)
{
	int	cx;
	int	cy;

	//verifica se está fora das paredes
	if (x < 0 || y < 0)
		return (1);
	
	//converte para numeros discretos, célula
	cx = (int)x;
	cy = (int)y;
	
	//verifica se é maior que a janela
	if (cx >= game->map.map_width || cy >= game->map.map_height)
		return (1);
	
	//verifica se é chão ou parede
	if (game->map.map[cy][cx] != '0')
		return (1);

	return (0);
}

static t_side	get_side(int prev_x, int cx, double ray_dx, double ray_dy)
{
	if (cx != prev_x)
	{
		if (ray_dx > 0)
			return (SIDE_W);
		return (SIDE_E);
	}
	if (ray_dy > 0)
		return (SIDE_N);
	return (SIDE_S);
}

t_hit	cast_ray(t_game *game, double ray_dx, double ray_dy)
{
	t_hit	hit;
	double	t;
	int		prev_x;
	int		is_wall;

	hit.dist = RAY_MAX_DIST;
	hit.side = SIDE_NONE;

	// defnimos a posição inicial do jogador
	hit.hit_x = game->play.play_x;
	hit.hit_y = game->play.play_y;
	prev_x = (int)game->play.play_x;
	t = 0;

	//precismao de um laço para rodar enquanto o pedaço do raio não bate em uma parede
	while (t < RAY_MAX_DIST)
	{
		//tem que incrementar os pedaços do raio com ele mesmo
		t += RAY_STEP;

		//fórmula da equação paramétrica
		hit.hit_x = game->play.play_x + ray_dx * t;
		hit.hit_y = game->play.play_y + ray_dy * t;
		
		//é parede?
		is_wall = is_wall_at(game, hit.hit_x, hit.hit_y);
		
		if (is_wall)
		{
			hit.dist = t;
			
			//pq eu preciso pegar o lado?
			hit.side = get_side(prev_x, (int)hit.hit_x, ray_dx, ray_dy);
			return (hit);
		}
		prev_x = (int)hit.hit_x;
	}
	return (hit);
}

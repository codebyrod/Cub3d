/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 15:55:32 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/31 23:32:49 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_side	get_side(t_ray *ray)
{
	// o raio atravessou a parede?
	// a coluna mudou entre o passo anterior e o atual?
	//atrevssou uma fronteira vertical?
	//coluna antes é igual a anterior? atravessou uma coluna
	if (ray->side == 0)
	{
		//se sim, verifica se é maior que 0
		if (ray->ray_dx > 0)
			return (SIDE_W);
		return (SIDE_E);
	}
	//se não mudou a coluna, muda a linha então
	if (ray->ray_dy > 0)
		return (SIDE_N);
	return (SIDE_S);
}

//vou fazer uma função pra capturar o valor da colisão
void	set_hit(t_game *game, t_ray *ray, t_hit *hit)
{
	hit->dist = ray->dist;
	hit->side = get_side(ray);
	hit->hit_x = game->play.play_x + ray->ray_dx * ray->dist;
	hit->hit_y = game->play.play_y + ray->ray_dy * ray->dist;
}

//precisamos de uma função para calcular o delta
//o delta vai calcular os intervalo entre as linhas da grade
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

//precisamos de uma função calcular em qual sentido o raio tá indo
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
		ray->side_dist_x = (ray->map_x + 1.0 - game->play.play_x) * ray->delta_dist_x;
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
		ray->step_y = 1;//CERTO
		ray->side_dist_y = (ray->map_y + 1.0 - game->play.play_y) * ray->delta_dist_y;
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

int	is_player_colliding(t_game *game, double x, double y)
{
	double radius;

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



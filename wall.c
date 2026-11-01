/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:42:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/31 23:51:13 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//vamos ter que calcular a distância perpendicular para evitar distorção olho de peixe
double calc_perp_dist(t_game *game, t_hit hit)
{
	//preciso da posição do jogador
	//preciso da distância da parede
	//preciso dos raios
	//preciso de uma formula que ajude a pegar o cateto que fica em perpendicular em relação a parede
	
	double	delta_x;
	double	delta_y;
	double	distance;

	//colisão

	//distância horizontal jogador e colisão
	delta_x = hit.hit_x - game->play.play_x;
	//distância vertical jogador e colisão
	delta_y = hit.hit_y - game->play.play_y;
	
	distance = delta_x * game->play.dir_x + delta_y * game->play.dir_y;

	//precisamos impedir que distancia seja zero ou negativas
	if (distance < 0.001)
		distance = 0.001;
	
	return (distance);

}


//vamos ter que calcular o tamanho da parede de acordo com o raio
//quanto menor o raio maior a parede
//por uma equestão de distância
int	calc_wall_height(double perp_dist)
{
	double focal_length;	
	double wall_height;
		
	//calcula a distancia focal a partir do FOV horizontal
	focal_length = (WIDTH / 2.0) / tan(FOV_DEG * M_PI / 360.0);
	
	//considera que cada parede tem uma unidade de altura
	wall_height = focal_length / perp_dist;

	//Limita a altura para evitar valores excessivos
	if (wall_height > HEIGHT * 2)
		wall_height = HEIGHT * 2;
	
	return ((int)wall_height);	
}

//agora precisamos desenhar a faixa vertical da parede
void	draw_wall_column(t_game *game, int x, int wall_h)
{
	int draw_start;
	int draw_end;
	int	y;
	
	//precisamos deixar a parede centralizada
	//vamos calc o tamanho da parede e dividir por doi
	draw_start = (HEIGHT - wall_h) / 2;
	draw_end = (HEIGHT + wall_h) / 2;

	//Impede que a parede ultrapassae o topo da tela.
	if (draw_start < 0)
		draw_start = 0;
	
	//Impede que a parede ultrapasse a parte inferior.
	if (draw_end > HEIGHT)
		draw_end = HEIGHT;
	
	//temos que controlar para ela não subir nem descer demais
	y = draw_start;

	//precisamos desenhar a parede
	while (y < draw_end)
	{
		my_pixel_put(&game->img, x, y, WALL);
		y++;
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

	//Calcula a direção do raio para a coluna atual
	get_camera_ray(game, x, &ray_dx, &ray_dy);
	
	//agora vamos pegar a primeira coluna atingida pelo raio
	hit = cast_ray(game, ray_dx, ray_dy);
	
	//agora vamos pegar a faixa perpendicular da colisão
	perp_dist = calc_perp_dist(game, hit);
	
	
	//precisamos pegar a altura através da distancia perpendicular
	wall_height = calc_wall_height(perp_dist);
	
	//e por ultimo vamos desenhar
	draw_wall_column(game, x, wall_height);

	return ;
}

void	render_walls(t_game *game)
{
	//uma função que vai correr de x até a largura da janela 
	//chamando a função de desenhar pela coluna

	int	x;
	
	x = 0;
	
	while (x < WIDTH)
	{
		//chamaa a função aqui
		render_wall_column(game, x);
		x++;
	}
	return ;
}
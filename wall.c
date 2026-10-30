/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:42:07 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/30 19:38:27 by rodrigo          ###   ########.fr       */
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
void	calc_wall_height(double perp_dist)
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

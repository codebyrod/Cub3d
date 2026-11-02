/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:35:11 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/01 20:14:12 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//construir o mapa

//construir o jogador
//calculo da movimentação do jogador

void	clear_screen(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y < HEIGHT / 2)
				my_pixel_put(&game->img, x, y, 0x555555);
			else
				my_pixel_put(&game->img, x, y, 0x888888);
			x++;
		}
		y++;
	}
}

void	test_ray(t_game *game)
{
	t_hit	hit;

	hit = cast_ray(game, game->play.dir_x, game->play.dir_y);
	printf("dist=%.3f side=%d hit=(%.3f, %.3f)\n",
		hit.dist, hit.side, hit.hit_x, hit.hit_y);
}

// Dispara um raio para cada coluna horizontal da tela.
// void	draw_camera_rays(t_game *game)
// {
// 	int		screen_x;
// 	double	ray_dx;
// 	double	ray_dy;

// 	screen_x = 0;
// 	// Percorre todas as colunas da janela.
// 	while (screen_x < WIDTH)
// 	{
// 		// Calcula a direção do raio correspondente à coluna.
// 		get_camera_ray(game, screen_x, &ray_dx, &ray_dy);
// 		// Executa o DDA e desenha a trajetória desse raio.
// 		draw_ray_debug(game, ray_dx, ray_dy);
// 		screen_x++;
// 	}
// }


// Desenha um raio individual no minimapa.
// void	draw_ray_debug(t_game *game, double ray_dx, double ray_dy)
// {
// 	t_hit	hit;
// 	double	ray_dist;
// 	int		px;
// 	int		py;

// 	// Executa o DDA e descobre onde o raio encontra uma parede.
// 	hit = cast_ray(game, ray_dx, ray_dy);
// 	ray_dist = 0;

// 	// Percorre o raio até a distância do ponto de colisão.
// 	while (ray_dist < hit.dist)
// 	{
		
// 		// Converte as coordenadas do mundo para pixels do minimapa.
// 		px = (game->play.play_x + ray_dx * ray_dist) * TILE_SIZE;
// 		py = (game->play.play_y + ray_dy * ray_dist) * TILE_SIZE;
		
// 		// Desenha o ponto atual do raio.
// 		my_pixel_put(&game->img, px, py, GREEN);
		
// 		// Avança um pequeno intervalo na trajetória do raio.
// 		ray_dist += 0.02;
// 	}
// }


void game_render(t_game *game)
{
	// map_render(game);
	// put_tile(game, 400, 0, 0, RED);
	// player_render(game);

	//escrevemos uma função de teste para ver o raio
	// draw_ray_debug(game, game->play.dir_x, game->play.dir_y);

	// Desenha todos os raios da câmera.
	// draw_camera_rays(game);

	//desenha o teto e o chão
	clear_screen(game);
	
	//Projeta as paredes na tela.
	render_walls(game);

	// test_ray(game);
	// handle_movement(game);
	mlx_put_image_to_window(game->connection,
		game->window,
		game->img.img_ptr,
		0, 0);
}

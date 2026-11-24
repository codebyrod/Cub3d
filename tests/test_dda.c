/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_dda.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:25:49 by rodrigo           #+#    #+#             */
/*   Updated: 2026/10/29 20:29:08 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * TESTES UNITARIOS DO ALGORITMO DDA
 *
 * Compilar junto com ray.c e ray_utils.c.
 * Os mapas usados aqui sao independentes do parser.
 */

static int	g_passed;
static int	g_failed;

static void	print_result(char *name, int passed)
{
	if (passed)
	{
		printf("[PASS] %s\n", name);
		g_passed++;
	}
	else
	{
		printf("[FAIL] %s\n", name);
		g_failed++;
	}
}

static int	almost_equal(double a, double b)
{
	return (fabs(a - b) < 0.0001);
}

static void	setup_game(t_game *game, char **map)
{
	memset(game, 0, sizeof(t_game));
	game->map.map = map;
	game->map.map_width = 7;
	game->map.map_height = 7;
	game->play.play_x = 3.5;
	game->play.play_y = 3.5;
}

/*
 * Mapa fechado:
 * As bordas sao paredes.
 * O interior e livre.
 */
static char	*g_closed_map[7] = {
	"1111111",
	"1000001",
	"1000001",
	"1000001",
	"1000001",
	"1000001",
	"1111111"
};

/*
 * 1. A posicao do jogador nao pode mudar.
 */
static void	test_player_position(void)
{
	t_game	game;
	t_hit	hit;
	double	initial_x;
	double	initial_y;

	setup_game(&game, g_closed_map);
	initial_x = game.play.play_x;
	initial_y = game.play.play_y;
	hit = cast_ray(&game, 1.0, 0.5);
	(void)hit;
	print_result("Posicao do jogador permanece inalterada",
		almost_equal(game.play.play_x, initial_x)
		&& almost_equal(game.play.play_y, initial_y));
}

/*
 * 2. Raios horizontais e verticais.
 * Direcao +X: encontra parede oeste.
 * Direcao +Y: encontra parede norte.
 */
static void	test_horizontal_ray(void)
{
	t_game	game;
	t_hit	hit;

	setup_game(&game, g_closed_map);
	hit = cast_ray(&game, 1.0, 0.0);
	print_result("Raio horizontal (+X)",
		almost_equal(hit.dist, 2.5)
		&& hit.side == SIDE_W);
}

static void	test_vertical_ray(void)
{
	t_game	game;
	t_hit	hit;

	setup_game(&game, g_closed_map);
	hit = cast_ray(&game, 0.0, 1.0);
	print_result("Raio vertical (+Y)",
		almost_equal(hit.dist, 2.5)
		&& hit.side == SIDE_N);
}

/*
 * 3. Raios diagonais.
 * O raio (1.0, 0.5) cruza primeiro a fronteira X.
 * A celula (4, 3) e uma parede.
 */
static void	test_diagonal_x_first(void)
{
	char	*map[7] = {
		"1111111",
		"1000001",
		"1000001",
		"1000101",
		"1000001",
		"1000001",
		"1111111"
	};
	t_game	game;
	t_hit	hit;

	setup_game(&game, map);
	hit = cast_ray(&game, 1.0, 0.5);
	print_result("Diagonal: fronteira X atravessada primeiro",
		almost_equal(hit.dist, 0.5)
		&& hit.side == SIDE_W
		&& almost_equal(hit.hit_x, 4.0)
		&& almost_equal(hit.hit_y, 3.75));
}

/*
 * O raio (0.5, 1.0) cruza primeiro a fronteira Y.
 * A celula (3, 4) e uma parede.
 */
static void	test_diagonal_y_first(void)
{
	char	*map[7] = {
		"1111111",
		"1000001",
		"1000001",
		"1000001",
		"1001001",
		"1000001",
		"1111111"
	};
	t_game	game;
	t_hit	hit;

	setup_game(&game, map);
	hit = cast_ray(&game, 0.5, 1.0);
	print_result("Diagonal: fronteira Y atravessada primeiro",
		almost_equal(hit.dist, 0.5)
		&& hit.side == SIDE_N
		&& almost_equal(hit.hit_x, 3.75)
		&& almost_equal(hit.hit_y, 4.0));
}

/*
 * 4. As quatro faces.
 */
static void	test_four_faces(void)
{
	t_game	game;
	t_hit	hit;
	int		passed;

	passed = 1;
	setup_game(&game, g_closed_map);
	hit = cast_ray(&game, 1.0, 0.0);
	if (hit.side != SIDE_W || !almost_equal(hit.dist, 2.5))
		passed = 0;
	hit = cast_ray(&game, -1.0, 0.0);
	if (hit.side != SIDE_E || !almost_equal(hit.dist, 2.5))
		passed = 0;
	hit = cast_ray(&game, 0.0, 1.0);
	if (hit.side != SIDE_N || !almost_equal(hit.dist, 2.5))
		passed = 0;
	hit = cast_ray(&game, 0.0, -1.0);
	if (hit.side != SIDE_S || !almost_equal(hit.dist, 2.5))
		passed = 0;
	print_result("Colisao nas quatro faces", passed);
}

/*
 * 5. Empate entre X e Y.
 * O codigo atual escolhe Y quando as distancias sao iguais.
 * A celula (3, 4) e parede; (4, 3) e livre.
 */
static void	test_corner_tie(void)
{
	char	*map[7] = {
		"1111111",
		"1000001",
		"1000001",
		"1000001",
		"1001001",
		"1000001",
		"1111111"
	};
	t_game	game;
	t_hit	hit;

	setup_game(&game, map);
	hit = cast_ray(&game, 1.0, 1.0);
	print_result("Canto: empate prioriza eixo Y",
		almost_equal(hit.dist, 0.5)
		&& hit.side == SIDE_N);
}

/*
 * 6. Mapa sem parede na borda direita.
 * O raio deve parar ao sair dos limites, sem acessar
 * uma posicao invalida do array.
 */
static void	test_map_boundary(void)
{
	char	*map[7] = {
		"0000000",
		"0000000",
		"0000000",
		"0000000",
		"0000000",
		"0000000",
		"0000000"
	};
	t_game	game;
	t_hit	hit;

	setup_game(&game, map);
	game.play.play_x = 5.5;
	game.play.play_y = 3.5;
	hit = cast_ray(&game, 1.0, 0.0);
	print_result("Limite do mapa sem acesso fora do array",
		almost_equal(hit.dist, 1.5)
		&& hit.side == SIDE_W);
}

/*
 * 7. Distancia para projecao.
 *
 * Direcao do jogador: (1, 0).
 * Raio: (1, 0.5).
 * Como ray_dir = dir + camera_plane * cameraX,
 * a distancia parametrica deve corresponder a
 * distancia perpendicular neste exemplo.
 */
static void	test_projection_distance(void)
{
	char	*map[7] = {
		"1111111",
		"1000001",
		"1000001",
		"1000001",
		"1000001",
		"1000001",
		"1111111"
	};
	t_game	game;
	t_hit	hit;
	double	perp_dist;

	setup_game(&game, map);
	hit = cast_ray(&game, 1.0, 0.5);
	perp_dist = (hit.hit_x - game.play.play_x);
	print_result("Distancia compativel com projecao perpendicular",
		almost_equal(hit.dist, 2.5)
		&& almost_equal(perp_dist, hit.dist));
}

int	main(void)
{
	printf("\n========== DDA UNIT TESTS ==========\n\n");
	test_player_position();
	test_horizontal_ray();
	test_vertical_ray();
	test_diagonal_x_first();
	test_diagonal_y_first();
	test_four_faces();
	test_corner_tie();
	test_map_boundary();
	test_projection_distance();
	printf("\n========== TEST SUMMARY ==========\n");
	printf("PASS: %d\n", g_passed);
	printf("FAIL: %d\n", g_failed);
	printf("TOTAL: %d\n", g_passed + g_failed);
	if (g_failed > 0)
		return (1);
	return (0);
}

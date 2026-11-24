
#include "cub3d.h"
#include <assert.h>

/*
 * Compara dois valores double considerando uma margem de erro.
 * Evita falhas causadas por pequenas imprecisões de ponto flutuante.
 */
static void	assert_close(double actual, double expected, double epsilon)
{
	assert(fabs(actual - expected) < epsilon);
}

/*
 * Prepara um mapa mínimo para inicializar o jogador.
 * A primeira célula livre será a posição inicial.
 */
static void	setup_game(t_game *game)
{
	static char	*map[] = {
		"00",
		"00",
		NULL
	};

	*game = (t_game){0};
	game->map.map = map;
	game->map.map_width = 2;
	game->map.map_height = 2;
}

/*
 * Teste 1:
 * Verifica a direção inicial e a inicialização do plano da câmera.
 */
static void	test_camera_initialization(void)
{
	t_game	game;
	double	expected_length;

	setup_game(&game);
	player_init(&game);

	expected_length = tan((FOV_DEG * M_PI / 180.0) / 2.0);

	// A direção inicial deve apontar para baixo.
	assert_close(game.play.dir_x, 0.0, 0.0001);
	assert_close(game.play.dir_y, 1.0, 0.0001);

	// O plano deve ser perpendicular à direção.
	assert_close(game.play.dir_x * game.play.cam_x
		+ game.play.dir_y * game.play.cam_y, 0.0, 0.0001);

	// O comprimento do plano deve corresponder ao FOV.
	assert_close(hypot(game.play.cam_x, game.play.cam_y),
		expected_length, 0.0001);

	// Com direção (0, 1), o plano deve apontar para X negativo.
	assert_close(game.play.cam_x, -expected_length, 0.0001);
	assert_close(game.play.cam_y, 0.0, 0.0001);

	printf("[OK] Inicializacao da camera\n");
}

/*
 * Teste 2:
 * Verifica se o raio central segue exatamente a direção do jogador.
 */
static void	test_center_ray(void)
{
	t_game	game;
	double	ray_dx;
	double	ray_dy;

	setup_game(&game);
	player_init(&game);

	get_camera_ray(&game, WIDTH / 2, &ray_dx, &ray_dy);

	assert_close(ray_dx, game.play.dir_x, 0.0001);
	assert_close(ray_dy, game.play.dir_y, 0.0001);

	printf("[OK] Raio central\n");
}

/*
 * Teste 3:
 * Verifica se os raios laterais possuem direções diferentes.
 */
static void	test_side_rays(void)
{
	t_game	game;
	double	left_dx;
	double	left_dy;
	double	right_dx;
	double	right_dy;

	setup_game(&game);
	player_init(&game);

	get_camera_ray(&game, 0, &left_dx, &left_dy);
	get_camera_ray(&game, WIDTH - 1, &right_dx, &right_dy);

	// O raio esquerdo deve apontar para X positivo.
	assert(left_dx > 0.0);

	// O raio direito deve apontar para X negativo.
	assert(right_dx < 0.0);

	// Ambos devem manter a mesma componente Y.
	assert_close(left_dy, right_dy, 0.0001);

	printf("[OK] Raios laterais\n");
}

/*
 * Teste 4:
 * Verifica a simetria dos raios em torno do centro.
 */
static void	test_ray_symmetry(void)
{
	t_game	game;
	double	left_dx;
	double	left_dy;
	double	right_dx;
	double	right_dy;

	setup_game(&game);
	player_init(&game);

	// As colunas 100 e 300 são simétricas em WIDTH = 400.
	get_camera_ray(&game, 100, &left_dx, &left_dy);
	get_camera_ray(&game, 300, &right_dx, &right_dy);

	// As componentes X devem ter sinais opostos.
	assert_close(left_dx, -right_dx, 0.0001);

	// As componentes Y devem ser iguais.
	assert_close(left_dy, right_dy, 0.0001);

	printf("[OK] Simetria dos raios\n");
}

/*
 * Teste 5:
 * Verifica se a abertura angular dos raios corresponde ao FOV.
 *
 * A primeira coluna representa -1 em cameraX.
 * A última coluna representa 1 - 2/WIDTH.
 * Portanto, a abertura entre os centros dos pixels extremos
 * será ligeiramente menor que o FOV teórico.
 */
static void	test_field_of_view(void)
{
	t_game	game;
	double	left_dx;
	double	left_dy;
	double	right_dx;
	double	right_dy;
	double	left_angle;
	double	right_angle;
	double	expected_fov;
	double	actual_fov;

	setup_game(&game);
	player_init(&game);

	get_camera_ray(&game, 0, &left_dx, &left_dy);
	get_camera_ray(&game, WIDTH - 1, &right_dx, &right_dy);

	// Calcula os ângulos dos raios em relação à direção central.
	left_angle = atan2(left_dx, left_dy);
	right_angle = atan2(right_dx, right_dy);

	// Calcula a abertura entre os raios extremos.
	actual_fov = left_angle - right_angle;

	// FOV esperado entre os limites teóricos -1 e +1.
	expected_fov = FOV_DEG * M_PI / 180.0;

	// A abertura entre os pixels extremos deve ser próxima de 66 graus.
	assert(fabs(actual_fov - expected_fov) < 0.01);

	printf("[OK] Campo de visao (FOV)\n");
}

/*
 * Teste 6:
 * Verifica se o tamanho dos raios muda conforme a coluna.
 *
 * Os raios laterais não devem ser normalizados.
 * Sua magnitude aumenta conforme se afastam do centro.
 */
static void	test_ray_magnitude(void)
{
	t_game	game;
	double	center_dx;
	double	center_dy;
	double	side_dx;
	double	side_dy;
	double	center_length;
	double	side_length;

	setup_game(&game);
	player_init(&game);

	get_camera_ray(&game, WIDTH / 2, &center_dx, &center_dy);
	get_camera_ray(&game, 0, &side_dx, &side_dy);

	center_length = hypot(center_dx, center_dy);
	side_length = hypot(side_dx, side_dy);

	// O raio central tem comprimento 1.
	assert_close(center_length, 1.0, 0.0001);

	// O raio lateral tem comprimento maior que 1.
	assert(side_length > center_length);

	printf("[OK] Magnitude dos raios\n");
}

/*
 * Executa todos os testes da câmera.
 */
int	main(void)
{
	printf("\n=== TESTES DO DIA 16 ===\n\n");

	test_camera_initialization();
	test_center_ray();
	test_side_rays();
	test_ray_symmetry();
	test_field_of_view();
	test_ray_magnitude();

	printf("\nTodos os testes passaram!\n");
	return (0);
}

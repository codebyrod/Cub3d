/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 21:52:34 by rodrigo           #+#    #+#             */
/*   Updated: 2026/09/20 21:42:53 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_press(int keycode, t_game *game)
{
	if (keycode == XK_w || keycode == XK_W)
		game->keys.w = BOOL_TRUE;
	else if (keycode == XK_a || keycode == XK_A)
		game->keys.a = BOOL_TRUE;
	else if (keycode == XK_s || keycode == XK_S)
		game->keys.s = BOOL_TRUE;
	else if (keycode == XK_d || keycode == XK_D)
		game->keys.d = BOOL_TRUE;
	else if (keycode == XK_x || keycode == XK_X)
		game->keys.x = BOOL_TRUE;
	else if (keycode == XK_Escape)
		game->keys.esc = BOOL_TRUE;
	else if (keycode == XK_Up)
		game->keys.a_up = BOOL_TRUE;
	else if (keycode == XK_Right)
		game->keys.a_right = BOOL_TRUE;
	else if (keycode == XK_Down)
		game->keys.a_bottom = BOOL_TRUE;
	else if (keycode == XK_Left)
		game->keys.a_left = BOOL_TRUE;
	printf("A tecla APERTADA foi %d\n", keycode);
	// printf("keycode recebido: %d\n", keycode);
	return (0);
}
int	handle_release(int keycode, t_game *game)
{
	if (keycode == XK_w || keycode == XK_W)
		game->keys.w = BOOL_FALSE;
	else if (keycode == XK_a || keycode == XK_A)
		game->keys.a = BOOL_FALSE;
	else if (keycode == XK_s || keycode == XK_S)
		game->keys.s = BOOL_FALSE;
	else if (keycode == XK_d || keycode == XK_D)
		game->keys.d = BOOL_FALSE;
	else if (keycode == XK_x || keycode == XK_X)
		game->keys.x = BOOL_FALSE;
	else if (keycode == XK_Escape)
		game->keys.esc = BOOL_FALSE;
	else if (keycode == XK_Up)
		game->keys.a_up = BOOL_FALSE;
	else if (keycode == XK_Right)
		game->keys.a_right = BOOL_FALSE;
	else if (keycode == XK_Down)
		game->keys.a_bottom = BOOL_FALSE;
	else if (keycode == XK_Left)
		game->keys.a_left = BOOL_FALSE;
	printf("A tecla SOLTA foi %d\n", keycode);
	return (0);
}

int	handle_movement(t_game *game)
{	
	double delta_x;
	double delta_y;
	double	magnitude;

	delta_x = 0;
	delta_y = 0;
	if (game->keys.w == BOOL_TRUE || game->keys.a_up == BOOL_TRUE)
		delta_y -= 0.001;
	if (game->keys.s == BOOL_TRUE || game->keys.a_bottom == BOOL_TRUE)
		delta_y += 0.001;
	if (game->keys.a == BOOL_TRUE || game->keys.a_left == BOOL_TRUE)
		delta_x -= 0.001;
	if (game->keys.d == BOOL_TRUE || game->keys.a_right == BOOL_TRUE)
		delta_x += 0.001;
	magnitude = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	if (magnitude > 0.001)
	{
		delta_x = (delta_x / magnitude) * 0.001;
		delta_y = (delta_y / magnitude) * 0.001;
	}
	game->play.play_x += delta_x;
	game->play.play_y += delta_y;
	game_render(game);
	return (0);
}

int	close_handler(t_game *game)
{
	mlx_destroy_image(game->connection, game->img.img_ptr);
	mlx_destroy_window(game->connection, game->window);
	mlx_destroy_display(game->connection);
	free(game->connection);
	exit (EXIT_SUCCESS);
}

void	events_init(t_game *game)
{
	// mlx_hook(game->window, KeyPress, KeyPressMask,
		// handle_movement,  game);
	
	//FAZER DOIS KEY_HOOK. PRESS E RELEASE
	
	//PRESS
	mlx_hook(game->window, KeyPress, KeyPressMask, handle_press, game);
	
	//RELEASE
	mlx_hook(game->window, KeyRelease, KeyReleaseMask, handle_release, game);
	
	//loop
	mlx_loop_hook(game->connection, handle_movement, game);
	printf("loop_hook aqui\n");
	mlx_hook(game->window, DestroyNotify,
		StructureNotifyMask, close_handler, game);
	// printf("events\n");
}
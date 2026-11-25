/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 21:52:34 by rodrigo           #+#    #+#             */
/*   Updated: 2026/11/25 03:41:02 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	// printf("A tecla SOLTA foi %d\n", keycode);
	return (0);
}



void	events_init(t_game *game)
{
	//FAZER DOIS KEY_HOOK. PRESS E RELEASE
	
	//PRESS
	mlx_hook(game->window, KeyPress, KeyPressMask, handle_press, game);
	
	//RELEASE
	mlx_hook(game->window, KeyRelease, KeyReleaseMask, handle_release, game);
	
	//loop
	mlx_loop_hook(game->connection, handle_movement, game);
	// printf("loop_hook aqui\n");
	mlx_hook(game->window, DestroyNotify,
		StructureNotifyMask, close_handler, game);
	// printf("events\n");
}
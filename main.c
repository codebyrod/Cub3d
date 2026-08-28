/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodrigo <rodrigo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 20:58:53 by rodrigo           #+#    #+#             */
/*   Updated: 2026/08/28 00:26:28 by rodrigo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"



int	main(void)
{
	t_game game;
	int		cub;
	
	cub = cub_init(game);
	if (!cub)
	{
		printf("Deu erro na inicialização do cub\n");
	}
	return (0);
}


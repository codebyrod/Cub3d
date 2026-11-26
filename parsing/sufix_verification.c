/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sufix_verification.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 16:28:44 by febranda          #+#    #+#             */
/*   Updated: 2026/11/26 16:50:53 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

int	sufix_verification(char *map)
{
	int		len;
	char	*sufix;

	len = ft_strlen(map);
	sufix = ".cub";
	map = &map[len - 4];
	while (*map)
	{
		if (*map == *sufix)
		{
			map++;
			sufix++;
		}
		else
			break ;
	}
	if (*map == '\0' && *sufix == '\0')
	{
		return (0);
	}
	return (1);
}

int	texture_sufix_verification(char *texture)
{
	int		len;
	char	*sufix;

	len = ft_strlen(texture);
	sufix = ".xpm";
	texture = &texture[len - 4];
	while (*texture)
	{
		if (*texture == *sufix)
		{
			texture++;
			sufix++;
		}
		else
			break ;
	}
	if (*texture == '\0' && *sufix == '\0')
	{
		return (0);
	}
	return (1);
}

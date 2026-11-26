/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_values.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 16:28:34 by febranda          #+#    #+#             */
/*   Updated: 2026/11/26 16:29:13 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

static int	parse_component(char **str, int *value)
{
	*value = 0;
	while (is_space(**str))
		(*str)++;
	if (!ft_isdigit(**str))
		return (1);
	while (ft_isdigit(**str))
	{
		*value = *value * 10 + (**str - '0');
		if (*value > 255)
			return (1);
		(*str)++;
	}
	while (is_space(**str))
		(*str)++;
	return (0);
}

int	parse_color(char *str, int *color)
{
	int	rgb[3];
	int	i;

	i = 0;
	while (i < 3)
	{
		if (parse_component(&str, &rgb[i]))
			return (1);
		if (i < 2 && *str != ',')
			return (1);
		if (i < 2)
			str++;
		i++;
	}
	if (*str)
		return (1);
	*color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	return (0);
}

int	check_texture_file(char *path)
{
	int		fd;
	char	c;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (1);
	if (read(fd, &c, 1) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

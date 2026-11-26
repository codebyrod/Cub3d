/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 16:11:53 by febranda          #+#    #+#             */
/*   Updated: 2026/11/26 17:18:12 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

static int	split_config(char *line, char **id, char **value)
{
	int	len;

	while (is_space(*line))
		line++;
	*id = line;
	while (*line && !is_space(*line))
		line++;
	if (!*line)
		return (1);
	*line = '\0';
	line++;
	while (is_space(*line))
		line++;
	*value = line;
	len = ft_strlen(line);
	while (len > 0 && is_space(line[len - 1]))
		len--;
	line[len] = '\0';
	return (**value == '\0');
}

static int	set_texture(char **dst, int *flag, char *path)
{
	if (*flag)
	{
		parse_error("Duplicate texture configuration");
		return (1);
	}
	if (texture_sufix_verification(path))
	{
		parse_error("Not a .xpm texture");
		return (1);
	}
	if (check_texture_file(path))
	{
		parse_error("Could not open a texture file");
		return (1);
	}
	*dst = ft_strdup(path);
	if (!*dst)
	{
		free(*dst);
		parse_error("Memory allocation failed");
		return (1);
	}
	*flag = 1;
	return (0);
}

static int	set_color(int *dst, int *flag, char *str)
{
	if (*flag)
	{
		parse_error("Duplicate color configuration");
		return (1);
	}
	if (parse_color(str, dst))
	{
		parse_error("Invalid color (expected R,G,B in 0-255)");
		return (1);
	}
	*flag = 1;
	return (0);
}

static int	set_config(char *id, char *value, t_data_pars *cfg)
{
	t_parser	*f;
	int			ret;

	f = &cfg->flags;
	if (!ft_strncmp(id, "NO", 3))
		ret = set_texture(&cfg->textures.north, &f->no_texture, value);
	else if (!ft_strncmp(id, "SO", 3))
		ret = set_texture(&cfg->textures.south, &f->so_texture, value);
	else if (!ft_strncmp(id, "WE", 3))
		ret = set_texture(&cfg->textures.west, &f->we_texture, value);
	else if (!ft_strncmp(id, "EA", 3))
		ret = set_texture(&cfg->textures.east, &f->ea_texture, value);
	else if (!ft_strncmp(id, "F", 2))
		ret = set_color(&cfg->floor_color, &f->has_floor, value);
	else if (!ft_strncmp(id, "C", 2))
		ret = set_color(&cfg->ceil_color, &f->has_ceil, value);
	else
	{
		parse_error("Unknown identifier in configuration section");
		ret = 1;
	}
	return (ret);
}

int	handle_config(char *line, t_data_pars *game_config)
{
	char		*id;
	char		*value;
	t_parser	*f;

	if (split_config(line, &id, &value))
	{
		parse_error("Invalid line (configuration expected)");
		return (1);
	}
	if (set_config(id, value, game_config))
		return (1);
	f = &game_config->flags;
	if (f->no_texture && f->so_texture && f->we_texture && f->ea_texture
		&& f->has_floor && f->has_ceil)
		f->config_ended = 1;
	return (0);
}

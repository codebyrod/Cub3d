/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 16:28:41 by febranda          #+#    #+#             */
/*   Updated: 2026/11/26 16:29:16 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

int	handle_line(char *line, t_data_pars *game_config)
{
	t_parser	*f;
	int			ret;

	f = &game_config->flags;
	if (is_blank(line))
	{
		if (f->map_started)
			f->map_ended = 1;
		return (0);
	}
	if (f->map_ended)
	{
		parse_error("Empty line or extra content inside or after the map");
		return (1);
	}
	if (!f->config_ended)
		ret = handle_config(line, game_config);
	else
	{
		f->map_started = 1;
		ret = handle_map_line(line, game_config);
	}
	return (ret);
}

static int	finish_parse(t_data_pars *game_config)
{
	int	ret;

	if (!game_config->flags.config_ended)
	{
		parse_error("Missing or incomplete configuration");
		return (1);
	}
	ret = validate_map(game_config);
	return (ret);
}

int	parse_file(t_data_pars *game_config, int fd)
{
	char	*line;
	int		ret;

	line = get_next_line(fd);
	while (line)
	{
		remove_newline(line);
		if (handle_line(line, game_config))
		{
			free(line);
			drain_gnl(fd);
			close(fd);
			return (1);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	ret = finish_parse(game_config);
	return (ret);
}

int	parse(char *filename, t_data_pars *game_config)
{
	int	fd;

	ft_bzero(game_config, sizeof(t_data_pars));
	if (sufix_verification(filename))
	{
		parse_error("Invalid map extension (expected .cub)");
		return (1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		parse_error("Could not open the map file");
		return (1);
	}
	if (parse_file(game_config, fd))
	{
		free_config(game_config);
		return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:14:47 by febranda          #+#    #+#             */
/*   Updated: 2026/11/25 18:23:42 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stddef.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include "../libft/libft.h"

typedef struct s_texture_pars
{
	char	*west;
	char	*east;
	char	*north;
	char	*south;
}	t_texture_pars;

typedef struct s_player_pars
{
	int		row;
	int		col;
	char	view_direction; // N, S, E, W
	int		count;
}	t_player_pars;

typedef struct s_parser
{
	int	config_ended;
	int	map_started;
	int	map_ended;
	int	has_ceil;
	int	has_floor;
	int	we_texture;
	int	ea_texture;
	int	no_texture;
	int	so_texture;
}	t_parser;

typedef struct s_map_pars
{
	char	**map_layout;
	int		height;
	int		width;
}	t_map_pars;

typedef struct s_data_pars
{
	int             floor_color;
	int             ceil_color;
	t_map_pars	    map;
	t_texture_pars  textures;
	t_player_pars   player_data;
	t_parser        flags;
}	t_data_pars;


int		parse(char *filename, t_data_pars *game_config);
int		parse_file(t_data_pars *game_config, int fd);
int		handle_line(char *line, t_data_pars *game_config);
int		handle_config(char *line, t_data_pars *game_config);
int		parse_color(char *str, int *color);
int		check_texture_file(char *path);
int		handle_map_line(char *line, t_data_pars *game_config);
int		count_rows(char **layout);
int		validate_map(t_data_pars *game_config);
int		setup_map_size(t_map_pars *map);
void	parse_error(char *msg);
int		is_space(char c);
int		is_blank(char *line);
void	remove_newline(char *line);
void	drain_gnl(int fd);
void	free_config(t_data_pars *game_config);
int		sufix_verification(char *map);

//INTEGRAÇÃO
// int		config_to_game(t_data_pars *cfg, t_game *game);
// void	free_game_map(t_game *game);
// void	clear_textures(t_game *game);

#endif

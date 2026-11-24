#include "../includes/parser.h"

static int	is_map_char(char c)
{
	return (c == ' ' || c == '0' || c == '1'
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	count_rows(char **layout)
{
	int	count;

	count = 0;
	while (layout && layout[count])
		count++;
	return (count);
}

static int	append_row(char ***layout, char *row)
{
	char	**new_layout;
	int		count;
	int		i;

	count = count_rows(*layout);
	new_layout = ft_calloc(count + 2, sizeof(char *));
	if (!new_layout)
		return (1);
	i = 0;
	while (i < count)
	{
		new_layout[i] = (*layout)[i];
		i++;
	}
	new_layout[i] = row;
	free(*layout);
	*layout = new_layout;
	return (0);
}

static int	check_map_line(char *line, t_data_pars *cfg)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_map_char(line[i]))
		{
			parse_error("Invalid character in the map");
			return (1);
		}
		if (ft_strchr("NSEW", line[i]))
		{
			line[i] = '0';
			cfg->player_data.count++;
			cfg->player_data.row = count_rows(cfg->map.map_layout);
			cfg->player_data.col = i;
			cfg->player_data.view_direction = line[i];
		}
		i++;
	}
	return (0);
}

int	handle_map_line(char *line, t_data_pars *game_config)
{
	char	*row;

	if (check_map_line(line, game_config))
		return (1);
	row = ft_strdup(line);
	if (!row)
	{
		parse_error("Memory allocation failed");
		return (1);
	}
	if (append_row(&game_config->map.map_layout, row))
	{
		free(row);
		parse_error("Memory allocation failed");
		return (1);
	}
	return (0);
}

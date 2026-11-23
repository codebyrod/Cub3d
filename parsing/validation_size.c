#include "../includes/parser.h"

static int	get_width(char **layout)
{
	int	width;
	int	len;
	int	i;

	width = 0;
	i = 0;
	while (layout[i])
	{
		len = ft_strlen(layout[i]);
		if (len > width)
			width = len;
		i++;
	}
	return (width);
}

static int	pad_rows(char **layout, int width)
{
	char	*padded;
	int		i;

	i = 0;
	while (layout[i])
	{
		padded = ft_calloc(width + 1, sizeof(char));
		if (!padded)
			return (1);
		ft_memset(padded, ' ', width);
		ft_memcpy(padded, layout[i], ft_strlen(layout[i]));
		free(layout[i]);
		layout[i] = padded;
		i++;
	}
	return (0);
}

int	setup_map_size(t_map_pars *map)
{
	int	width;
	int	height;

	height = count_rows(map->map_layout);
	width = get_width(map->map_layout);
	if (pad_rows(map->map_layout, width))
	{
		parse_error("Memory allocation failed");
		return (1);
	}
	map->height = height;
	map->width = width;
	return (0);
}

#include "../includes/parser.h"

static void	free_layout(char **layout)
{
	int	i;

	if (!layout)
		return ;
	i = 0;
	while (layout[i])
	{
		free(layout[i]);
		i++;
	}
	free(layout);
}

void	free_config(t_data_pars *game_config)
{
	free(game_config->textures.north);
	free(game_config->textures.south);
	free(game_config->textures.west);
	free(game_config->textures.east);
	free_layout(game_config->map.map_layout);
	game_config->textures.north = NULL;
	game_config->textures.south = NULL;
	game_config->textures.west = NULL;
	game_config->textures.east = NULL;
	game_config->map.map_layout = NULL;
}

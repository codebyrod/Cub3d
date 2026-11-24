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

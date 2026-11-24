/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:30:19 by febranda          #+#    #+#             */
/*   Updated: 2025/07/31 18:56:07 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str2;
	int		size;
	int		i;

	size = ft_strlen(s);
	str2 = malloc(size + 1);
	if (str2 == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		str2[i] = s[i];
		i++;
	}
	str2[i] = '\0';
	return (str2);
}

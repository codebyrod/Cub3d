/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:22:18 by febranda          #+#    #+#             */
/*   Updated: 2025/07/31 17:30:12 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*temp;

	temp = 0;
	while (*s)
	{
		if (*s == ((char)c))
			temp = (char *)s;
		s++;
	}
	if (temp)
		return (temp);
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

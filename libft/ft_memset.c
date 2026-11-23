/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:34:23 by febranda          #+#    #+#             */
/*   Updated: 2025/07/30 15:44:17 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	byte;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	byte = (unsigned char)c;
	while (i < n)
	{
		ptr[i] = byte;
		i++;
	}
	return (s);
}

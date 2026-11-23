/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:00:59 by febranda          #+#    #+#             */
/*   Updated: 2025/08/04 15:37:13 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new_str;
	unsigned int	len;
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	while (i < len)
	{
		new_str[i] = (*f)(i, s[i]);
		i++;
	}
	new_str[len] = '\0';
	return (new_str);
}

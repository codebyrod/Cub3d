/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:00:29 by febranda          #+#    #+#             */
/*   Updated: 2025/08/02 12:41:51 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*new_string;
	size_t		str_len;

	str_len = ft_strlen(s);
	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	new_string = malloc(len * sizeof(char) + 1);
	if (!new_string)
		return (NULL);
	ft_memcpy(new_string, s + start, len);
	new_string[len] = '\0';
	return (new_string);
}

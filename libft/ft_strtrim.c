/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:05 by febranda          #+#    #+#             */
/*   Updated: 2025/08/05 15:48:07 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t			start;
	size_t			end;
	char			*new_string;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
	{
		start++;
	}
	while (end > start && ft_strrchr(set, s1[end - 1]))
	{
		end--;
	}
	new_string = malloc((end - start) + 1);
	if (!new_string)
		return (NULL);
	ft_strlcpy(new_string, s1 + start, (end - start) + 1);
	return (new_string);
}

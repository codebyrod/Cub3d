/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:17:26 by febranda          #+#    #+#             */
/*   Updated: 2026/11/22 22:48:08 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// char	*ft_strchr(const char *str, int letter)
// {
// 	if (!str)
// 		return (NULL);
// 	while (*str)
// 	{
// 		if (*str == (char)letter)
// 			return ((char *)str);
// 		str++;
// 	}
// 	if ((char)letter == '\0')
// 		return ((char *)str);
// 	return (NULL);
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*new_str;
// 	size_t	len;
// 	size_t	i;
// 	size_t	j;

// 	i = -1;
// 	j = -1;
// 	if (!s1)
// 		s1 = ft_strdup("");
// 	len = ft_strlen(s1) + ft_strlen(s2);
// 	new_str = ft_calloc(len + 1, sizeof(char));
// 	if (!new_str)
// 		return (NULL);
// 	while (s1[++i])
// 		new_str[i] = s1[i];
// 	while (s2[++j])
// 		new_str[i + j] = s2[j];
// 	new_str[i + j] = '\0';
// 	free((void *)s1);
// 	return (new_str);
// }

// size_t	ft_strlen(const char *str)
// {
// 	size_t	len;

// 	len = 0;
// 	while (*str)
// 	{
// 		len++;
// 		str++;
// 	}
// 	return (len);
// }

// void	*ft_calloc(size_t nmemb, size_t size)
// {
// 	size_t			total;
// 	size_t			i;
// 	unsigned char	*ptr;

// 	i = 0;
// 	if (nmemb == 0 || size == 0)
// 	{
// 		ptr = malloc(0);
// 		return (ptr);
// 	}
// 	if (nmemb > (size_t)-1 / size)
// 		return (NULL);
// 	total = nmemb * size;
// 	ptr = malloc(total);
// 	if (!ptr)
// 		return (NULL);
// 	while (i < total)
// 	{
// 		ptr[i] = '\0';
// 		i++;
// 	}
// 	return ((void *)ptr);
// }

// char	*ft_strdup(const char *str)
// {
// 	char	*new_str;
// 	int		i;
// 	int		len;

// 	len = ft_strlen(str);
// 	new_str = malloc(len + 1);
// 	if (new_str == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (str[i])
// 	{
// 		new_str[i] = str[i];
// 		i++;
// 	}
// 	new_str[i] = '\0';
// 	return (new_str);
// }

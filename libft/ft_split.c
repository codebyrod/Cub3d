/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:48:34 by febranda          #+#    #+#             */
/*   Updated: 2025/08/14 18:01:44 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static size_t	ft_word_count(char const *s, char c);
static char		*ft_get_word( char const *s, size_t end, char **arr, size_t j);
static int		ft_fill_array(char **arr, char const *s, char c);

char	**ft_split(char const *s, char c)
{
	char	**array;

	if (!s)
		return (NULL);
	array = (char **)malloc((ft_word_count(s, c) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	if (!ft_fill_array(array, s, c))
		return (NULL);
	return (array);
}

static int	ft_fill_array(char **arr, char const *s, char c)
{
	size_t	i;
	size_t	j;

	j = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s != c && *s)
		{
			i = 0;
			while (s[i] && s[i] != c)
				i++;
			arr[j] = ft_get_word(s, i + 1, arr, j);
			if (!arr[j])
				return (0);
			j++;
			s += i;
		}
	}
	arr[j] = NULL;
	return (1);
}

static char	*ft_get_word(char const *s, size_t size, char **arr, size_t j)
{
	char	*word;

	word = malloc(size);
	if (!word)
	{
		while (j > 0)
		{
			free(arr[j - 1]);
			j--;
		}
		free (arr);
		return (NULL);
	}
	ft_strlcpy(word, s, size);
	return (word);
}

static size_t	ft_word_count(char const *s, char c)
{
	size_t	counter;

	counter = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s != c && *s)
			counter++;
		while (*s && *s != c)
			s++;
	}
	return (counter);
}

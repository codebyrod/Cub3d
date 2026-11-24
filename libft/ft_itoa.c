/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:26:36 by febranda          #+#    #+#             */
/*   Updated: 2025/08/04 19:55:28 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len_itoa(long nb);
static char	*ft_fill_str(char *arr, int len, int signal, long nb);

char	*ft_itoa(int n)
{
	long	nb;
	char	*str;
	int		signal;
	int		len;

	nb = n;
	len = ft_len_itoa(n);
	signal = 0;
	if (n < 0)
	{
		nb = -nb;
		signal = 1;
		len++;
	}
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str = ft_fill_str(str, len, signal, nb);
	return (str);
}

static int	ft_len_itoa(long nb)
{
	int	len;

	if (nb == 0)
		return (1);
	len = 0;
	while (nb)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

static char	*ft_fill_str(char *str, int len, int signal, long nb)
{
	str[len] = '\0';
	len--;
	while (len >= signal)
	{
		str[len] = (nb % 10) + 48;
		nb = nb / 10;
		len--;
	}
	if (signal)
		str[0] = '-';
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rosousa- <rosousa-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:20:33 by febranda          #+#    #+#             */
/*   Updated: 2026/11/22 22:48:44 by rosousa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include "../libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 7
# endif

char	*get_next_line(int fd);
char	*fill_buffer(char *buffer, int fd);
// char	*ft_strchr(const char *str, int letter);
// char	*ft_strjoin(char const *s1, char const *s2);
// size_t	ft_strlen(const char *str);
// void	*ft_calloc(size_t nmemb, size_t size);
char	*extract_line(char	*buffer);
char	*rearrange_buffer(char *buffer);
char	*ft_strdup(const char *str);
// void	ft_bzero(void *s, size_t n);

#endif

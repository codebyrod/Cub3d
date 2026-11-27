/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febranda <febranda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:03:58 by febranda          #+#    #+#             */
/*   Updated: 2026/11/27 13:49:07 by febranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*next_line;

	if (fd < 0)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	if (BUFFER_SIZE <= 0)
		return (NULL);
	buffer = fill_buffer(buffer, fd);
	if (!buffer || (!buffer[0]))
	{
		if (buffer)
			free(buffer);
		buffer = NULL;
		return (NULL);
	}
	next_line = extract_line(buffer);
	buffer = rearrange_buffer(buffer);
	return (next_line);
}

static char	*append_and_clean(char *buffer, char *temp, int bytes_read)
{
	char	*joined;

	temp[bytes_read] = '\0';
	joined = ft_strjoin(buffer, temp);
	free(buffer);
	if (!joined)
	{
		free(temp);
		return (NULL);
	}
	return (joined);
}

char	*fill_buffer(char *buffer, int fd)
{
	int		bytes_read;
	char	*temp;

	temp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!temp)
		return (NULL);
	if (!buffer)
		buffer = ft_strdup("");
	bytes_read = 1;
	while (buffer && !ft_strchr(buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, temp, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			free(temp);
			return (NULL);
		}
		buffer = append_and_clean(buffer, temp, bytes_read);
	}
	free(temp);
	return (buffer);
}

char	*extract_line(char	*buffer)
{
	char	*new_line;
	int		len_new_line;
	int		i;

	i = 0;
	len_new_line = 1;
	while (buffer[i])
	{
		i++;
		len_new_line++;
		if (buffer[i] == '\n')
			break ;
	}
	i = 0;
	new_line = ft_calloc((len_new_line + 1), sizeof(char));
	while (buffer[i])
	{
		new_line[i] = buffer[i];
		if (buffer[i] == '\n')
			break ;
		i++;
	}
	return (new_line);
}

char	*rearrange_buffer(char *buffer)
{
	char	*str;
	int		i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
		{
			i++;
			break ;
		}
		i++;
	}
	str = ft_strdup(&buffer[i]);
	free(buffer);
	return (str);
}

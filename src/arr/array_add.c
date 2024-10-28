/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   array_add.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:51:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/28 18:29:34 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

char	**array_add(char **arr, char *string)
{
	char	**new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (arr == NULL)
	{
		new = calloc(2, sizeof(char*));
		if (new == NULL)
		{
			perror("array_add");
			exit(EXIT_FAILURE);
		}
		new[0] = string;
		new[1] = NULL;
	}
	else
	{
		while (arr[i] != NULL)
			i++;
		new = calloc(i + 2, sizeof(char*));
		if (new == NULL)
		{
			perror("array_add");
			exit(EXIT_FAILURE);
		}
		while (j < i)
		{
			new[j] = arr[j];
			j++;
		}
		new[j] = string;
		j++;
		new[j] = NULL;
		free(arr);
	}
	return (new);
}


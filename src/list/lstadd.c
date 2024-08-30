/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstadd.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:31:33 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/30 16:05:07 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

int	lstadd(t_data *data)
{
	t_mallocs *new;

	new = lstnew(data);
	if (new == NULL)
		return (1);
	lstadd_back(&data->mallocs, new);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstgive_backtrace.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:31:33 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/28 17:44:03 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

t_mallocs *lstgive_node(t_data *data, int i)
{
	t_mallocs	*temp;
	int			j;

	temp = data->mallocs;
	j = 0;
	while (j < i)
	{
		temp = temp->next;
		j++;
	}
	return (temp);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstnew.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:51:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/29 14:31:03 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

t_mallocs	*lstnew(t_data *data)
{
	t_mallocs	*new_node;

	new_node = calloc(1, sizeof(t_mallocs));
	if (new_node == NULL)
		return (NULL);
	new_node->num = data->malloc_count;
	new_node->next = NULL;
	return (new_node);
}

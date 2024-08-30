/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstsize.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:56:22 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/30 15:50:15 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

int	lstsize(t_mallocs *lst)
{
	int			size;
	t_mallocs	*temp;

	size = 0;
	temp = lst;
	while (temp != NULL)
	{
		temp = temp->next;
		size++;
	}
	return (size);
}

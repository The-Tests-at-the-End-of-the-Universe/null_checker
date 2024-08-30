/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstprint.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:56:22 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/30 16:07:53 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

void	lstprint(t_mallocs *lst)
{
	t_mallocs	*temp;

	temp = lst;
	while (temp != NULL)
	{
		printf("malloc num: %d\n", temp->num);
		temp = temp->next;
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstlast.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:46:33 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/29 22:23:46 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

t_mallocs	*lstlast(t_mallocs	*lst)
{
	t_mallocs	*last;

	last = calloc(1, sizeof(t_mallocs));
	if (last == NULL)
		return (NULL);
	last = lst;
	if (last == NULL)
		return (NULL);
	if (last->next == NULL)
		return (last);
	while (last != NULL)
	{
		if (last->next->next == NULL)
		{
			last = last->next;
			return (last);
		}
		last = last->next;
	}
	return (last);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lst_secondlast.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:46:33 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/29 22:17:34 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

t_mallocs	*lst_second_last(t_mallocs	*lst)
{
	t_mallocs	*last;

	last = lst;
	if (last == NULL)
		return (NULL);
	if (last->next == NULL)
		return (last);
	if (last->next->next == NULL)
		return (last);
	while (last != NULL)
	{
		if (last->next->next == NULL)
			return (last);
		last = last->next;
	}
	return (last);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstadd_back.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:31:33 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/30 16:04:44 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

void	lstadd_back(t_mallocs	**lst, t_mallocs	*new)
{
	t_mallocs	*last;

	last = lstlast((*lst));
	if (last == NULL)
		(*lst) = new;
	else
		last->next = new;
}

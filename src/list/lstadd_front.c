/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstadd_front.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:35:18 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/30 15:45:12 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

void	lstadd_front(t_mallocs	**lst, t_mallocs	*new)
{
	new->next = (*lst);
	(*lst) = new;
}

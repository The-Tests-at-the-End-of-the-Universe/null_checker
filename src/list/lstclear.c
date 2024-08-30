/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstclear.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:43:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/30 15:46:21 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

void	lstclear(t_mallocs	**lst)
{
	t_mallocs	*temp_frt;
	t_mallocs	*temp_bck;

	if (!lst)
		return ;
	temp_frt = (*lst);
	temp_bck = (*lst);
	while (temp_bck)
	{
		temp_frt = temp_frt->next;
		free(temp_bck);
		temp_bck = temp_frt;
	}
	(*lst) = 0;
}

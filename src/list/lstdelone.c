/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lstdelone.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/23 13:44:39 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/30 15:48:52 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <zaphod.h>

void	lstdelone(t_mallocs	*lst)
{
	free(lst);
	lst = NULL;
}

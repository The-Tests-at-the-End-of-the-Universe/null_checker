/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   zaphod.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/21 23:03:23 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/21 17:46:08 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZAPHOD_H
# define ZAPHOD_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>


typedef struct s_mallocs
{
	int					num;
	int					fail;
	struct s_mallocs	*next;
} t_mallocs;


typedef struct s_data
{
	int					malloc_count;
	struct s_mallocs	*mallocs;
	int					null_check;
	int					null_check_count;
}	t_data;

t_mallocs	*lstnew(t_data *data);
void		lstadd_front(t_mallocs **lst, t_mallocs *new);
int			lstsize(t_mallocs *lst);
t_mallocs	*lstlast(t_mallocs *lst);
void		lstadd_back(t_mallocs **lst, t_mallocs *new);
void		lstdelone(t_mallocs *lst);
void		lstclear(t_mallocs **lst);
int			lstadd(t_data *data);
void		lstprint(t_mallocs *lst);

#endif

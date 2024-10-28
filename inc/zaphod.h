/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   zaphod.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/21 23:03:23 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/28 17:44:23 by mynodeus      ########   odam.nl         */
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
	int					num; //number assigned to the malloc
	char				**backtrace; // backtrace in which function this malloc was called
	int					nptrs; // number of pointers, important for printing the backtrace
	int					fail; // if set to 1, then this malloc will return NULL instead of real pointer
	struct s_mallocs	*next; // pointer to next malloc
} t_mallocs;


typedef struct s_data
{
	int					malloc_count; // amount of mallocs hijacked program called.
	struct s_mallocs	*mallocs; // pointer to malloc linkedlist
	int					null_check; // if set to 1, then program will run malloc test
	int					null_check_count; // integer used to keep track of which malloc has to be nulled
	int					exit_code; // exit code for end of program
}	t_data;

void		print_backtrace(t_mallocs *node);
void		add_backtrace(t_mallocs *node);
t_mallocs	*lstgive_node(t_data *data, int i);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   zaphod.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mynodeus <mynodeus@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/21 23:03:23 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/28 19:16:12 by mynodeus      ########   odam.nl         */
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
#include <stdarg.h>
#include <fcntl.h>

# define RED   "\x1B[31m"
# define GRN   "\x1B[1;32m"
# define YEL   "\x1B[33m"
# define BLU   "\x1B[34m"
# define MAG   "\x1B[35m"
# define BMAG   "\x1B[1;35m"
# define CYN   "\x1B[36m"
# define BCYN   "\x1B[1;36m"
# define WHT   "\x1B[37m"
# define RESET "\x1B[0m"

typedef struct s_mallocs
{
	int					num; //number assigned to the malloc
	char				**backtrace; // backtrace in which function this malloc was called
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
	int					debug; // if set to 1, then program will print internal debug messages
	int					print_bt; // if set to 1, then program will print backtrace of failed malloc
}	t_data;

void		print_backtrace(t_mallocs *node);
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
char		**array_add(char **arr, char *string);

#endif

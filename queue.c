/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 10:55:31 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 18:46:09 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	has_prio(t_heap_node a, t_heap_node b)
{
	if (a.priority != b.priority)
		return (a.priority < b.priority);
	return (a.coder_id < b.coder_id);
}

static void	heapify_up(t_dongle *dongle, int i)
{
	t_heap_node	tmp;
	int			parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!has_prio(dongle->heap[i], dongle->heap[parent]))
			break ;
		tmp = dongle->heap[i];
		dongle->heap[i] = dongle->heap[parent];
		dongle->heap[parent] = tmp;
		i = parent;
	}
}

static void	heapify_down(t_dongle *dongle, int i)
{
	int			smallest;
	int			left;
	int			right;
	t_heap_node	tmp;

	while (1)
	{
		smallest = i;
		left = 2 * i + 1;
		right = 2 * i + 2;
		if (left < dongle->heap_size && has_prio(dongle->heap[left],
				dongle->heap[smallest]))
			smallest = left;
		if (right < dongle->heap_size && has_prio(dongle->heap[right],
				dongle->heap[smallest]))
			smallest = right;
		if (smallest == i)
			break ;
		tmp = dongle->heap[i];
		dongle->heap[i] = dongle->heap[smallest];
		dongle->heap[smallest] = tmp;
		i = smallest;
	}
}

void	push_heap(t_dongle *dongle, int coder_id, long long priority)
{
	int	i;

	i = 0;
	while (i < dongle->heap_size)
	{
		if (dongle->heap[i].coder_id == coder_id)
			return ;
		i++;
	}
	if (dongle->heap_size >= dongle->capacity)
		return ;
	dongle->heap[dongle->heap_size].coder_id = coder_id;
	dongle->heap[dongle->heap_size].priority = priority;
	dongle->heap_size++;
	heapify_up(dongle, dongle->heap_size - 1);
}

void	pop_heap(t_dongle *dongle)
{
	if (dongle->heap_size == 0)
		return ;
	dongle->heap[0] = dongle->heap[dongle->heap_size - 1];
	dongle->heap_size--;
	heapify_down(dongle, 0);
}

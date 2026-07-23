/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 10:55:31 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 00:47:35 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	heapify_up(t_dongle *dongle, int i)
{
	t_heap_node	tmp;
	int			parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (dongle->heap[i].priority >= dongle->heap[parent].priority)
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
		if (left < dongle->heap_size && dongle->heap[left].priority
			< dongle->heap[smallest].priority)
			smallest = left;
		if (right < dongle->heap_size && dongle->heap[right].priority
			< dongle->heap[smallest].priority)
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

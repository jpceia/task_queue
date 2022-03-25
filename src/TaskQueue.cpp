/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:39:20 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 21:21:31 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TaskQueue.hpp"
#include "Task.hpp"
#include "LockGuard.hpp"
#include <pthread.h>

TaskQueue::TaskQueue()
{
}

TaskQueue::TaskQueue(TaskQueue const &rhs)
{
    (void)rhs;
}

TaskQueue::~TaskQueue()
{
}

TaskQueue& TaskQueue::operator=(TaskQueue const &rhs)
{
    (void)rhs;
    return *this;
}

void TaskQueue::push(Task *task)
{
    if (task == NULL)
        return ;
    else
    {
        LockGuard lock(_mutex);
        _queue.push(task);
    }
    _cond.signal();
}

Task* TaskQueue::pop(bool wait)
{
    LockGuard lock(_mutex);
    while (_queue.empty() && wait)
        _cond.wait();
    if (_queue.empty())
        return NULL;
    Task *task = _queue.front();
    _queue.pop();
    return task;
}

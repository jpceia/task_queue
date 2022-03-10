/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:39:20 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/10 23:09:38 by jpceia           ###   ########.fr       */
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
    {
        LockGuard lock(_mutex);
        if (_stopped)
            return;
        _queue.push(task);
    }
    _cond.signal();
}

Task* TaskQueue::pop()
{
    LockGuard lock(_mutex);
    while (_queue.empty())
    {
        if (_stopped)
            return NULL;
        _cond.wait(_mutex);
    }
    Task *task = _queue.front();
    _queue.pop();
    return task;
}

bool TaskQueue::isStopped()
{
    return _stopped;
}

void TaskQueue::stop()
{
    _stopped = true;
    _cond.broadcast();
}

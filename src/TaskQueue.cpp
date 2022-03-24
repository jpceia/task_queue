/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:39:20 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 15:18:04 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TaskQueue.hpp"
#include "Task.hpp"
#include "LockGuard.hpp"
#include <pthread.h>

TaskQueue::TaskQueue() :
    _stopped(false)
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
    if (_stopped)
        return;
    if (task == NULL)
    {
        _stopped = true;
        _cond.broadcast();
        return;
    }
    else
    {
        LockGuard lock(_mutex);
        _queue.push(task);
    }
    _cond.signal();
}

Task* TaskQueue::pop()
{
    LockGuard lock(_mutex);

        _cond.wait();
    if (_queue.empty())
        return NULL;
    Task *task = _queue.front();
    _queue.pop();
    return task;
}

bool TaskQueue::isStopped()
{
    return _stopped;
}

void TaskQueue::reset()
{
    // Delete all pending tasks
    while (!_queue.empty())
    {
        Task* task = _queue.front();
        if (task != NULL)
            delete task;
        _queue.pop();
    }
    _stopped = false;
}

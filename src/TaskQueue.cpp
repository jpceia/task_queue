/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:39:20 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 00:13:38 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TaskQueue.hpp"
#include "Task.hpp"
#include "LockGuard.hpp"
#include <pthread.h>
#include <iostream>

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
        #ifdef DEBUG
        std::cout << "Task " << task->getId() << " is pushed to taskQueue" << std::endl;
        #endif
        _queue.push(task);
    }
    _cond.signal();
}

void TaskQueue::push(const std::vector<Task *>& tasks)
{
    if (tasks.empty())
        return ;
    else
    {
        LockGuard lock(_mutex);
        for (std::vector<Task *>::const_iterator it = tasks.begin();
            it != tasks.end(); ++it)
        {
            if (*it != NULL)
                _queue.push(*it);
        }
    }
    _cond.broadcast();
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:39:20 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 02:55:19 by jpceia           ###   ########.fr       */
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
    while (!_queue.empty())
    {
        Task *task = _queue.front();
        _queue.pop();
        delete task;
    }
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
        LockGuard lock(_push_mtx);
        #ifdef DEBUG
        std::cout << "Task " << task->getId() << " is pushed to taskQueue" << std::endl;
        #endif
        _queue.push(task);
    }
    _cv.signal();
}

void TaskQueue::push(const std::vector<Task *>& tasks)
{
    if (tasks.empty())
        return ;
    else
    {
        LockGuard lock(_push_mtx);
        for (std::vector<Task *>::const_iterator it = tasks.begin();
            it != tasks.end(); ++it)
        {
            if (*it != NULL)
                _queue.push(*it);
        }
    }
    _cv.broadcast();
}

Task* TaskQueue::pop()
{
    bool wait = false;
    return pop(wait);
}

Task* TaskQueue::pop(bool& wait)
{
    LockGuard lock(_pop_mtx);
    while (_queue.empty() && wait)
        _cv.wait();
    if (_queue.empty())
        return NULL;
    Task *task = _queue.front();
    _queue.pop();
    return task;
}

bool TaskQueue::empty() const
{
    return _queue.empty();
}

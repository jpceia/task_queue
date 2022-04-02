/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:39:20 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 04:49:50 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TaskQueue.hpp"
#include "Task.hpp"
#include "Atomic.hpp"
#include "LockGuard.hpp"
#include <pthread.h>
#include <iostream>


namespace wm
{

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
        LockGuard lock(_pushMtx);
        #ifdef DEBUG
        std::cout << "Task " << task->getId() << " is pushed to taskQueue" << std::endl;
        #endif
        _queue.push(task);
        _cv.signal();
    }
    
}

void TaskQueue::push(const std::vector<Task *>& tasks)
{
    if (tasks.empty())
        return ;
    else
    {
        LockGuard lock(_pushMtx);
        for (std::vector<Task *>::const_iterator it = tasks.begin();
            it != tasks.end(); ++it)
        {
            if (*it != NULL)
                _queue.push(*it);
        }
        _cv.broadcast();
    }
}

Task* TaskQueue::pop()
{
    bool wait = false;
    return pop(wait);
}

Task* TaskQueue::pop(const Atomic<bool>& wait)
{
    LockGuard lock(_popMtx);
    while (this->empty() && wait)
        _cv.wait(_popMtx);
    if (this->empty())
        return NULL;
    LockGuard lockEmpty(_pushMtx);
    Task *task = _queue.front();
    _queue.pop();
    return task;
}

bool TaskQueue::empty() const
{
    LockGuard lock(_pushMtx);
    return _queue.empty();
}

void TaskQueue::clear()
{
    while (true)
    {
        Task *task = this->pop();
        if (task == NULL)
            break ;
        delete task;
    }
}

} // namespace wm

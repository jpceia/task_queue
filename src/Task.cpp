/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:33:08 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/04 09:02:00 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "sync/LockGuard.hpp"
#include "safe/Set.hpp"
#include "safe/Queue.hpp"
#include <iostream>
#include <vector>


namespace wm
{

Task::Task() :
    _locked(false)
{
}

Task::~Task()
{
}

bool Task::isLocked() const
{
    return _locked;
}

bool Task::isLocked(Task *task)
{
    return task->isLocked();
}

void Task::lock()
{
    _locked = true;
}

void Task::lock(Task *task)
{
    task->lock();
}

void Task::moveDeps(SafeSet<Task>& taskSet, SafeQueue<Task>& taskQueue)
{
    for (SafeSet<Node>::const_iterator it = _dependents.begin();
        it != _dependents.end(); ++it)
    {
        Task *task = dynamic_cast<Task *>(*it);
        task->_dependencies.erase(this);
        task->_moveIfReady(taskSet, taskQueue);
    }
}

// Must be locked before calling this function
bool Task::_moveIfReady(SafeSet<Task>& taskSet, SafeQueue<Task>& taskQueue)
{
    if (!this->isReady())
        return false;
    #ifdef DEBUG
    std::cout << "Task " << this->getId() << " is ready to pass to taskQueue" << std::endl;
    #endif
    if (!taskSet.erase(this))
    {
        #ifdef DEBUG
        std::cerr << "/!\\ Task" << this->getId() << " not found in taskSet"
            << std::endl;
        #endif
        return false;
    }
    #ifdef DEBUG
    std::cout << "Task " << this->getId() << " is removed from taskSet" << std::endl;
    #endif
    taskQueue.push(this);
    #ifdef DEBUG
    std::cout << "Task " << this->getId() << " passed to taskQueue" << std::endl;
    #endif
    return true;
}

} // namespace wm

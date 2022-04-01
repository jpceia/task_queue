/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:33:08 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/01 03:08:53 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "LockGuard.hpp"
#include "TaskSet.hpp"
#include "TaskQueue.hpp"
#include <iostream>
#include <vector>


Task::Task() :
    _locked(false)
{
}

Task::~Task()
{
    if (!_dependencies.empty())
        std::cerr << "Task " << this->getId() << " is being deleted while still having dependencies" << std::endl;
    for (TaskSet::const_iterator it = _dependents.begin();
        it != _dependents.end(); ++it)
    {
        Task *task = *it;
        task->_dependencies.erase(this);
    }
}

bool Task::isReady() const
{
    return _dependencies.empty();
}

TaskSet Task::getDependents() const
{
    return _dependents;
}

void Task::addDependency(Task *task)
{
    if (_locked)
    {
        std::cerr << "Task::addDependency: cannot add dependency to locked task"
            << std::endl;
        return ;
    }
    if (_dependencies.insert(task))
        task->_dependents.insert(this);
    else
        std::cerr << "Task::addDependency: dependency already exists" << std::endl;
}

void Task::lock()
{
    _locked = true;
}

void Task::lock(const std::vector<Task *>& tasks)
{
    for (std::vector<Task *>::const_iterator it = tasks.begin();
        it != tasks.end(); ++it)
        (*it)->lock();
}

void Task::moveDeps(TaskSet& taskSet, TaskQueue& taskQueue)
{
    for (TaskSet::const_iterator it = _dependents.begin();
        it != _dependents.end(); ++it)
    {
        Task *task = *it;
        task->_dependencies.erase(this);
        task->_moveIfReady(taskSet, taskQueue);
    }
}

// Must be locked before calling this function
bool Task::_moveIfReady(TaskSet& taskSet, TaskQueue& taskQueue)
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

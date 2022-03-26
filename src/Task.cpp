/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:33:08 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 04:12:47 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "LockGuard.hpp"
#include <iostream>
#include <vector>

unsigned int Task::_taskCount = 0;

Task::Task() :
    _id(_taskCount++)
{
}

Task::~Task()
{
    for (Set::const_iterator it = _dependents.begin();
        it != _dependents.end(); ++it)
    {
        Task *task = *it;
        LockGuard lock(task->_depMtx);
        task->_dependencies.erase(this);
    }
}

bool Task::isReady() const
{
    return _dependencies.empty();
}

Task::Set Task::getDependents() const
{
    return _dependents;
}

unsigned int Task::getId() const
{
    return _id;
}


void Task::addDependency(Task *task)
{
    if (_locked)
    {
        std::cerr << "Task::addDependency: cannot add dependency to locked task"
            << std::endl;
        return ;
    }
    _dependencies.insert(task);
    task->_dependents.insert(this);
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

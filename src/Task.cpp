/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:33:08 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 03:25:21 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "LockGuard.hpp"
#include <iostream>

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

void Task::addDependency(Task *task)
{
    _dependencies.insert(task);
    task->_dependents.insert(this);
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

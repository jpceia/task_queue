/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:33:08 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 23:34:13 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "LockGuard.hpp"
#include <iostream>

Task::Task()
{
}

Task::~Task()
{
    for (Set::const_iterator it = _dependents.begin();
        it != _dependents.end(); ++it)
    {
        Task *task = *it;
        LockGuard lock(task->_mutex);
        task->_dependencies.erase(this);
    }
}

void Task::add_dependency(Task *task)
{
    _dependencies.insert(task);
    task->_dependents.insert(this);
}

bool Task::isLocked() const
{
    return !_dependencies.empty();
}

Task::Set Task::getDependents() const
{
    return _dependents;
}

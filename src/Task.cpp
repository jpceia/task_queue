/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:33:08 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 20:38:37 by jpceia           ###   ########.fr       */
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
        (*it)->_remove_dependency(this);
}

void Task::add_dependency(Task *task)
{
    _dependencies.insert(task);
    task->_add_dependent(this);
}

void Task::_add_dependent(Task *task)
{
    LockGuard(task->_mutex);
    task->_dependents.insert(task);
}

bool Task::isLocked() const
{
    return !_dependencies.empty();
}

void Task::_remove_dependency(Task *task)
{
    if (!_dependencies.erase(task))
        std::cerr << "Task::_remove_dependency: task not found" << std::endl;
}

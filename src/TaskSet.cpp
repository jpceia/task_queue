/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskSet.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:40 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 20:47:33 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TaskSet.hpp"
#include "Task.hpp"

TaskSet::TaskSet(Task::Set task_set) :
    _set(task_set)
{
}

TaskSet::TaskSet(const TaskSet& rhs) :
    _set(rhs._set)
{
}

TaskSet::~TaskSet()
{
}

TaskSet& TaskSet::operator=(const TaskSet& rhs)
{
    if (this != &rhs)
        _set = rhs._set;
    return *this;
}

size_t TaskSet::erase(Task *task)
{
    LockGuard lock(_mutex);
    return _set.erase(task);
}

bool TaskSet::insert(Task *task)
{
    LockGuard lock(_mutex);
    return _set.insert(task).second;
}

bool TaskSet::empty() const
{
    return _set.empty();
}

void TaskSet::clear()
{
    for (Task::Set::const_iterator it = _set.begin(); it != _set.end(); ++it)
        delete *it;
}

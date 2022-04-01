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

TaskSet::TaskSet()
{
}

TaskSet::TaskSet(const TaskSet& rhs) :
    _set(rhs._set)
{
}

TaskSet::~TaskSet()
{
    while (!_set.empty())
        delete *_set.begin();
}

TaskSet& TaskSet::operator=(const TaskSet& rhs)
{
    if (this != &rhs)
        _set = rhs._set;
    return *this;
}

std::vector<Task*> TaskSet::moveUnlockedTasks()
{
    std::vector<Task *> tasks;
    Task::Set::iterator it = _set.begin();
    while (it != _set.end())
    {
        if ((*it)->isReady())
        {
            tasks.push_back(*it);
            _set.erase(it++);
        }
        else
            ++it;
    }
    return tasks;
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

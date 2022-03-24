/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskPool.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:40 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 18:46:36 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TaskPool.hpp"


TaskPool::TaskPool(Task::Set task_set) :
    _set(task_set)
{
}

TaskPool::TaskPool(const TaskPool& rhs) :
    _set(rhs._set)
{
}

TaskPool::~TaskPool()
{
}

TaskPool& TaskPool::operator=(const TaskPool& rhs)
{
    if (this != &rhs)
        _set = rhs._set;
    return *this;
}

void TaskPool::erase(Task* task)
{
    LockGuard lock(_mutex);
    _set.erase(task);
}

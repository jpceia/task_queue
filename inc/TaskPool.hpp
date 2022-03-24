/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskPool.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:45 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 18:54:46 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKPOOL_HPP
# define TASKPOOL_HPP

# include "Task.hpp"
# include "Mutex.hpp"
# include "LockGuard.hpp"
# include <set>


class TaskPool
{
public:
    TaskPool(Task::Set task_set = Task::Set());
    TaskPool(const TaskPool& rhs);
    virtual ~TaskPool();
    TaskPool& operator=(const TaskPool& rhs);

    void erase(Task *task);
    void reset();

private:
    Mutex _mutex;
    Task::Set _set;
};

#endif

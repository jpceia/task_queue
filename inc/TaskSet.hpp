/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskSet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:45 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 20:50:40 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKSET_HPP
# define TASKSET_HPP

# include "Task.hpp"
# include "Mutex.hpp"
# include "LockGuard.hpp"
# include <set>


class TaskSet
{
public:
    TaskSet(Task::Set task_set = Task::Set());
    TaskSet(const TaskSet& rhs);
    virtual ~TaskSet();
    TaskSet& operator=(const TaskSet& rhs);

    void erase(Task *task);
    void clear();

private:
    Mutex _mutex;
    Task::Set _set;
};

#endif

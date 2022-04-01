/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskSet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:45 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/01 03:09:36 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKSET_HPP
# define TASKSET_HPP

# include "Mutex.hpp"
# include <set>
# include <vector>

class Task;

class TaskSet
{
public:
    TaskSet();
    TaskSet(const TaskSet& rhs);
    virtual ~TaskSet();
    TaskSet& operator=(const TaskSet& rhs);

    std::vector<Task*> moveUnlockedTasks();
    size_t erase(Task *task);
    bool insert(Task *task);
    bool empty() const;
    void clear();

private:
    Mutex _mutex;
    Task::Set _set;
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 05:16:17 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP

# include <set>
# include <vector>
# include <pthread.h>
# include "Mutex.hpp"


class TaskSet;
class TaskQueue;

class Task
{
public:
    typedef std::set<Task *> Set;
    Task();
    virtual ~Task();
    virtual void run() = 0;

    bool isReady() const;
    bool isLocked() const;
    Set getDependents() const;
    unsigned int getId() const;

    void addDependency(Task *task);
    void lock();
    static void lock(const std::vector<Task *>& tasks);
    void moveDeps(TaskSet& taskSet, TaskQueue& taskQueue);

private:
    // Helper methods
    bool _moveIfReady(TaskSet& taskSet, TaskQueue& taskQueue);
    
    // Private attributes
    Set _dependencies;
    Set _dependents;
    Mutex _depMtx;
    const unsigned int _id;
    bool _locked;
    
    // static variable to count the number of tasks
    static unsigned int _taskCount;
};

#endif

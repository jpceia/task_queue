/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 04:20:42 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP

# include <set>
# include <vector>
# include <pthread.h>
# include "Mutex.hpp"
# include "BaseTask.hpp"

class TaskSet;
class TaskQueue;

class Task : public BaseTask
{
public:
    typedef std::set<Task *> Set;
    Task();
    virtual ~Task();

    bool isReady() const;
    bool isLocked() const;
    Set getDependents() const;

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
    bool _locked;
};

#endif

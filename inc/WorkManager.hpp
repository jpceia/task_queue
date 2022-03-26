/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:21:00 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 00:13:08 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKMANAGER_HPP
# define WORKMANAGER_HPP

# include "Mutex.hpp"
# include "LockGuard.hpp"
# include "ConditionVariable.hpp"
# include "Task.hpp"
# include "TaskSet.hpp"
# include "TaskQueue.hpp"
# include <vector>

class WorkManager
{
public:
    WorkManager(int numWorkers);
    ~WorkManager();

    void start();
    void wait();
    void push_task(Task* task);
    void push_task(const std::vector<Task *>& tasks);

private:
    // Non-copyable
    WorkManager(WorkManager const &rhs);
    WorkManager &operator=(WorkManager const &rhs);

    // thread functions
    static void *WorkerThread(void *ptr);

    // Helper function to delete an task and remove its dependents dependencies
    void _finish_task(Task *task);
    
    // Private attributes
    // Queue with tasks ready to be executed
    TaskQueue _taskQueue;

    // Set with tasks that can be executed at the moment
    TaskSet _lockedTasks;

    // vector of threads (workers)
    std::vector<pthread_t> _workers;

    // Boolean to indicate if the workers are running
    bool _working;

    // Boolean to indicate if the manager is waiting for tasks
    bool _accepting_work;
};

#endif

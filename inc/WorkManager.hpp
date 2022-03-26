/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:21:00 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 05:26:36 by jpceia           ###   ########.fr       */
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
    void pushTask(Task* task);
    void pushTask(const std::vector<Task *>& tasks);

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

    // Set with tasks that can NOT be executed at the moment
    TaskSet _taskPool;

    // vector of threads (workers)
    std::vector<pthread_t> _workers;

    // Boolean to indicate if the workers are running
    bool _working;

    // Boolean to indicate if the manager is waiting for tasks
    bool _acceptingWork;
};

#endif

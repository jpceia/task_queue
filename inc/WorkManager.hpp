/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:21:00 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/04 09:02:17 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKMANAGER_HPP
# define WORKMANAGER_HPP

# include "sync/Mutex.hpp"
# include "sync/Atomic.hpp"
# include "sync/ConditionVariable.hpp"
# include "safe/Set.hpp"
# include "safe/Queue.hpp"
# include "Task.hpp"
# include <vector>

namespace wm
{

class WorkManager
{
public:
    WorkManager(int numWorkers);
    virtual ~WorkManager();

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
    SafeQueue<Task> _taskQueue;

    // Set with tasks that can NOT be executed at the moment
    SafeSet<Task> _taskPool;

    // vector of threads (workers)
    std::vector<pthread_t> _workers;

    // Boolean to indicate if the workers are running
    bool _working;

    // Boolean to indicate if the manager is waiting for tasks
    Atomic<bool> _acceptingWork;
};

} // namespace wm

#endif

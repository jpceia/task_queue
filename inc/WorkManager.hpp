/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:21:00 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 23:45:18 by jpceia           ###   ########.fr       */
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

    void _finish_task(Task *task);
    
    // Private attributes
    TaskQueue _taskQueue;
    TaskSet _lockedTasks;
    std::vector<pthread_t> _workers;
    bool _working;
    bool _accepting_work;
};

#endif

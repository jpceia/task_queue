/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:21:00 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 15:36:54 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mutex.hpp"
#include "LockGuard.hpp"
#include "ConditionVariable.hpp"
#include "Task.hpp"
#include "TaskQueue.hpp"
#include <vector>

void* WorkerThread(void *ptr);

class WorkManager
{
public:
    WorkManager(int numWorkers);
    ~WorkManager();

    void init();
    void wait();
    void push_task(Task* task);
    void reset();
    
private:
    // Non-copyable
    WorkManager(WorkManager const &rhs);
    WorkManager &operator=(WorkManager const &rhs);
    
    // Private attributes
    TaskQueue _taskQueue;
    std::vector<pthread_t> _workers;
    bool _running;
};

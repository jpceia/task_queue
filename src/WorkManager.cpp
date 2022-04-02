/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:23:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 06:31:10 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "SafeQueue.hpp"
#include "Atomic.hpp"
#include "WorkManager.hpp"
#include <iostream>


namespace wm
{

void* WorkManager::WorkerThread(void *ptr)
{
    WorkManager* wm = (WorkManager *)ptr;
    SafeQueue<Task>& taskQueue = wm->_taskQueue;
    SafeSet<Task>& taskPool = wm->_taskPool;
    const Atomic<bool>& wait = wm->_acceptingWork;

    while (true)
    {
        Task *task = taskQueue.pop(wait);
        if (!task)
            break ;
        try 
        {
            task->run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        task->moveDeps(taskPool, taskQueue);
        delete task;
    }
    return NULL;
}

WorkManager::WorkManager(int numWorkers) :
    _workers(numWorkers),
    _working(false),
    _acceptingWork(true)
{
}

WorkManager::WorkManager(WorkManager const &rhs) :
    _workers(rhs._workers),
    _working(rhs._working),
    _acceptingWork(rhs._acceptingWork)
{
    *this = rhs;
}

WorkManager::~WorkManager()
{
    this->wait();
    _taskPool.clear();
    _taskQueue.clear();
}

WorkManager& WorkManager::operator=(WorkManager const &rhs)
{
    (void)rhs;
    return *this;
}

void WorkManager::wait()
{
    if (!_working)
        return ;
    _acceptingWork = false;
    for (std::vector<pthread_t>::const_iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_join(*it, NULL);
    _working = false;
    _acceptingWork = true;
}

void WorkManager::start()
{
    if (_working)
    {
        std::cerr << "WorkManager::start() called while already started" << std::endl;
        return ;
    }
    // Check which tasks are ready to pass to taskQueue (no dependencies)
    SafeSet<Task> readyTasks = _taskPool.filter_and_remove(Task::isReady);
    readyTasks.apply(Task::lock);
    _taskQueue.push(readyTasks.begin(), readyTasks.end());
    
    // Start workers
    for (std::vector<pthread_t>::iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_create(&*it, NULL, WorkerThread, (void*)this);
    _working = true;
}

void WorkManager::pushTask(Task *task)
{
    if (!_acceptingWork)
    {
        std::cerr << "WorkManager::push_task() called while not accepting work" << std::endl;
        return ;
    }
    if (task == NULL)
    {
        std::cerr << "WorkManager::push_task() called with NULL task" << std::endl;
        return ;
    }
    if (_working && task->isReady())
    {
        task->lock();
        _taskQueue.push(task);
    }
    else
    {
        _taskPool.insert(task);
    }
}

void WorkManager::pushTask(const std::vector<Task *>& tasks)
{
    if (!_acceptingWork)
    {
        std::cerr << "WorkManager::push_task() called while not accepting work" << std::endl;
        return ;
    }
    for (std::vector<Task *>::const_iterator it = tasks.begin();
        it != tasks.end(); ++it)
    {
        if (*it == NULL)
        {
            std::cerr << "WorkManager::push_task() called with NULL task" << std::endl;
            return ;
        }
        this->pushTask(*it);
    }
}

} // namespace wm
